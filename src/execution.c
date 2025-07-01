/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:20:20 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/01 02:55:31 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_redirects_pipe(t_cmd **tokens, t_pipedata *p)
{
	while (tokens[p->index] && tokens[p->index]->type != PIPE)
	{
		if (tokens[p->index]->type == OUTPUT)
		{
			if (p->outfile)
				close(p->outfile);
			p->outfile = open(tokens[p->index + 1]->str, OUTPUT_CONF, 0644);
		}
		if (tokens[p->index]->type == APPEND)
		{
			if (p->outfile)
				close(p->outfile);
			p->outfile = open(tokens[p->index + 1]->str, APPEND_CONF, 0644);
		}
		if (tokens[p->index]->type == INPUT)
		{
			if (p->infile)
				close(p->infile);
			p->infile = open(tokens[p->index + 1]->str, INPUT_CONF);
			if (p->infile < 0)
				perror("open");
		}
		p->index++;
	}
}

static void	wait_for_children(t_pipedata *p, int status, int *pids)
{
	int	i;

	i = 0;
	while (i < p->pipe_count + 1)
	{
		ignore();
		if (waitpid(pids[i], &status, 0) < 0)
			exit(1);
		child_died(status);
		catcher();
		i++;
	}
}

static void	close_unused_pipes(t_pipedata *p, int index)
{
	if (index > 0)
		close(p->pipefd[index - 1][READ]);
	if (index < p->pipe_count)
		close(p->pipefd[index][WRITE]);
}

static void	init_pipes(t_pipedata *p)
{
	int	i;

	p->pipefd = arena_malloc(sizeof(int *) * (p->pipe_count));
	i = 0;
	while (i < p->pipe_count)
		p->pipefd[i++] = arena_malloc(sizeof(int) * 2);
}

static void	setup_cmd_to_execute(t_cmd **tokens, t_pipedata *p)
{
	char	**cmd;
	size_t	size;
	size_t	i;
	size_t	arg_index;

	while (tokens[p->cmd_index] && tokens[p->cmd_index]->type != STRING)
		p->cmd_index++;
	cmd = mini_split(tokens[p->cmd_index]->str, ' ');
	size = p->cmd_index;
	while (tokens[size] && tokens[size]->type == FILES)
		size++;
	i = 0;
	while (cmd[i++])
		size++;
	p->cmd_args = arena_malloc((size - p->cmd_index) * sizeof(char *));
	i = 0;
	arg_index = 0;
	while (cmd[i])
		p->cmd_args[i++] = mini_strdup(cmd[arg_index++]);
	arg_index = p->cmd_index;
	if (tokens[arg_index]->next == FILES)
		arg_index++;
	while (tokens[arg_index] && tokens[arg_index]->type == FILES)
		p->cmd_args[i++] = mini_strdup(tokens[arg_index++]->str);
	p->cmd_args[i] = NULL;
}

static void	first_pipe(t_pipedata *p)
{
	if (dup2(p->infile, STDIN_FILENO) < 0)
		perror("dup2");
	if (p->infile != STDIN_FILENO)
		close(p->infile);
	if (dup2(p->pipefd[p->pipe_index][WRITE], STDOUT_FILENO) < 0)
		perror("dup2");
}

static void	last_pipe(t_pipedata *p)
{
	if (dup2(p->pipefd[p->pipe_index - 1][READ], STDIN_FILENO) < 0)
		perror("dup2");
	if (dup2(p->outfile, STDOUT_FILENO) < 0)
		perror("dup2");
	if (p->outfile != STDOUT_FILENO)
		close(p->outfile);
}

static void	middle_pipe(t_pipedata *p)
{
	if (dup2(p->pipefd[p->pipe_index - 1][READ], STDIN_FILENO) < 0)
		perror("dup2");
	if (dup2(p->pipefd[p->pipe_index][WRITE], STDOUT_FILENO) < 0)
		perror("dup2");
}

static void	child_process(t_cmd **tokens, t_pipedata *p, char **env)
{
	char	*path;

	if (p->pipe_index == 0)
		first_pipe(p);
	else if (p->pipe_index == p->pipe_count)
		last_pipe(p);
	else
		middle_pipe(p);
	close_unused_pipes(p, p->pipe_index);
	path = get_bin_path(tokens[p->cmd_index]->str, env);
	if (access(p->cmd_args[0], X_OK) >= 0)
		if (execve(p->cmd_args[0], p->cmd_args, env) < 0)
			exit(1);
	if (execve(path, p->cmd_args, env) < 0)
		exit(1);
}

static void setup_child(t_cmd **tokens, t_pipedata *p, char **env)
{
	t_pipedata	local_p;
	
	local_p = *p;
	local_p.index = p->index;
	local_p.cmd_index = p->index;
	local_p.pipe_index = i;
	check_redirects_pipe(tokens, &local_p);
	setup_cmd_to_execute(tokens, &local_p);
	child_process(tokens, &local_p, env);
}

static void find_next_cmd_index(t_cmd **tokens, t_pipeline *p)
{
    while (tokens[p->index] && tokens[p->index]->type != PIPE)
			p->index++;
		while (tokens[p->index] && tokens[p->index]->type == PIPE)
			p->index++;
}

static void	exec_pipeline(t_cmd **tokens, t_pipedata *p, char **env)
{
	int			status;
	int			*pids;
	int			i;

	pids = arena_malloc(sizeof(int) * (p->pipe_count + 1));
	status = 0;
	i = 0;
	while (i < p->pipe_count + 1)
	{
		if (i < p->pipe_count)
			if (pipe(p->pipefd[i]) < 0)
				perror("pipe");
		reset_sig();
		pids[i] = fork();
		if (pids[i] == 0)
			setup_child(tokens, p, env);
		close_unused_pipes(p, i);
		find_next_cmd_index(tokens, p);
		p->pipe_index++;
		i++;
	}
	wait_for_children(p, status, pids);
}

void	setup_pipeline(t_cmd **tokens, char **env)
{
	t_pipedata	*p;
	int			i;

	p = arena_malloc(sizeof(t_pipedata));
	p->index = 0;
	p->pipe_count = 0;
	p->cmd_index = 0;
	p->pipe_index = 0;
	p->infile = dup(STDIN_FILENO);
	p->outfile = dup(STDOUT_FILENO);
	i = 0;
	while (tokens[i])
		if (tokens[i++]->type == PIPE)
			p->pipe_count++;
	init_pipes(p);
	exec_pipeline(tokens, p, env);
}

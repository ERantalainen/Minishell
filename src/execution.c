/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:20:20 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/01 14:30:57 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	open_file(t_cmd **tokens, t_pipedata *p, int settings, int file)
{
	if (file)
		close(file);
	if (tokens[p->index]->type == INPUT || tokens[p->index]->type == HERE_DOC)
		file = open(tokens[p->index + 1]->str, settings);
	else
		file = open(tokens[p->index + 1]->str, settings, 0644);
}

static void	check_redirects_pipe(t_cmd **tokens, t_pipedata *p)
{
	while (tokens[p->index] && tokens[p->index]->type != PIPE)
	{
		if (tokens[p->index]->type == OUTPUT)
			open_file(tokens, p, OUTPUT_CONF, p->outfile);
		else if (tokens[p->index]->type == APPEND)
			open_file(tokens, p, APPEND_CONF, p->outfile);
		else if (tokens[p->index]->type == INPUT)
			open_file(tokens, p, INPUT_CONF, p->infile);
		else if (tokens[p->index]->type == HERE_DOC)
			open_file(tokens, p, INPUT_CONF, p->infile);
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
	{
		p->pipefd[i] = arena_malloc(sizeof(int) * 2);
		i++;
	}
}

static void	setup_cmd_to_execute(t_cmd **tokens, t_pipedata *p)
{
	char	**split_cmd;
	size_t	arg_total_count;
	size_t	args_array_index;
	size_t	token_index;

	while (tokens[p->cmd_index] && tokens[p->cmd_index]->type != STRING && tokens[p->cmd_index]->type != BUILTIN)
		p->cmd_index++;
	split_cmd = mini_split(tokens[p->cmd_index]->str, ' ');
	arg_total_count = p->cmd_index + 1;
	while (tokens[arg_total_count] && tokens[arg_total_count]->type == FILES)
		arg_total_count++;
	args_array_index = 0;
	while (split_cmd[args_array_index++])
		arg_total_count++;
	p->cmd_args = arena_malloc((arg_total_count - p->cmd_index) * sizeof(char *));
	args_array_index = 0;
	token_index = 0;
	while (split_cmd[args_array_index])
		p->cmd_args[args_array_index++] = mini_strdup(split_cmd[token_index++]);
	token_index = p->cmd_index;
	if (tokens[token_index]->next == FILES)
		token_index++;
	while (tokens[token_index] && tokens[token_index]->type == FILES)
		p->cmd_args[args_array_index++] = mini_strdup(tokens[token_index++]->str);
	p->cmd_args[args_array_index] = NULL;
}

static void	setup_pipes(int in, int out, int close_in, int close_out)
{
	if (dup2(in, STDIN_FILENO) < 0)
		perror("dup2");
	if (close_in && in != STDIN_FILENO)
		close(in);
	if (dup2(out, STDOUT_FILENO) < 0)
		perror("dup2");
	if (close_out && out != STDOUT_FILENO)
		close(out);
}

static void	child_process(t_cmd **tokens, t_pipedata *p, char **env)
{
	char	*path;

	if (p->pipe_count == 0)
		setup_pipes(p->infile, p->outfile, 1, 1);
	else if (p->pipe_index == 0)
		setup_pipes(p->infile, p->pipefd[p->pipe_index][WRITE], 1, 0);
	else if (p->pipe_index == p->pipe_count)
		setup_pipes(p->pipefd[p->pipe_index - 1][READ], p->outfile, 0, 1);
	else
		setup_pipes(p->pipefd[p->pipe_index - 1][READ], p->pipefd[p->pipe_index][WRITE], 0, 0);
	close_unused_pipes(p, p->pipe_index);
	if (tokens[p->cmd_index]->type == BUILTIN)
	{
		build_handler(tokens);
		exit(1);
	}
	if (access(p->cmd_args[0], X_OK) >= 0)
		if (execve(p->cmd_args[0], p->cmd_args, env) < 0)
			exit(1);
	path = get_bin_path(tokens[p->cmd_index]->str, env);
	if (execve(path, p->cmd_args, env) < 0)
		exit(1);
}

static void	setup_child(t_cmd **tokens, t_pipedata *p, char **env, int i)
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

static void	find_next_cmd_index(t_cmd **tokens, t_pipedata *p)
{
	while (tokens[p->index] && tokens[p->index]->type != PIPE)
		p->index++;
	while (tokens[p->index] && tokens[p->index]->type == PIPE)
		p->index++;
}

static void	exec_pipeline(t_cmd **tokens, t_pipedata *p, char **env)
{
	int	status;
	int	*pids;
	int	i;

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
			setup_child(tokens, p, env, i);
		find_next_cmd_index(tokens, p);
		close_unused_pipes(p, i);
		p->pipe_index++;
		i++;
	}
	wait_for_children(p, status, pids);
}

void	execution(t_cmd **tokens, char **env)
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

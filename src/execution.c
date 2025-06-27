/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:20:20 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/27 20:56:40 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_redirects_pipe(t_cmd **tokens, t_pipedata *p)
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
		}
		p->index++;
	}
}
// fix cat cat etc
void	setup_cmd_to_execute(t_cmd **tokens, t_pipedata *p)
{
	char	**cmd;
	size_t	size;
	size_t	i;

	while (tokens[p->cmd_index])
	{
		if (tokens[p->cmd_index]->type == STRING)
			break ;
		p->cmd_index++;
	}
	cmd = mini_split(tokens[p->cmd_index]->str, ' ');
	size = p->cmd_index;
	while (tokens[size]->next == FILES || tokens[size]->next == STRING)
		size++;
	i = 0;
	while (cmd[i++] != NULL)
		size++;
	p->cmd_args = arena_malloc(size * sizeof(char *));
	i = 0;
	while (cmd[i] != NULL)
	{
		p->cmd_args[i] = mini_strdup(cmd[i]);
		i++;
	}
	while (tokens[p->cmd_index] && tokens[p->cmd_index + 1])
	{
		if (tokens[p->cmd_index + 1]->type != FILES)
			break ;
		p->cmd_args[i++] = mini_strdup(tokens[p->cmd_index + 1]->str);
		p->cmd_index++;
	}
	p->cmd_args[i] = NULL;
}

void	child_process(t_cmd **tokens, t_pipedata *p, char **env)
{
	char	*path;

	if (p->pipe_index == 0)
	{
		if (p->infile)
			dup2(p->infile, STDIN_FILENO);
		dup2(p->pipefd[0][WRITE], STDOUT_FILENO);
	}
	else if (p->pipe_index == p->pipe_count)
	{
		if (p->pipe_count == 1)
			dup2(p->pipefd[0][READ], STDIN_FILENO);
		else
			dup2(p->pipefd[1][READ], STDIN_FILENO);
		if (p->outfile)
			dup2(p->outfile, STDOUT_FILENO);
	}
	else
	{
		dup2(p->pipefd[0][READ], STDIN_FILENO);
		dup2(p->pipefd[1][WRITE], STDOUT_FILENO);
	}
	close(p->pipefd[0][0]);
	close(p->pipefd[0][1]);
	close(p->pipefd[1][0]);
	close(p->pipefd[1][1]);
	ft_fprintf(STDERR_FILENO, "%s\n", p->cmd_args[0]);
	if (access(p->cmd_args[0], X_OK) >= 0)
		if (execve(p->cmd_args[0], p->cmd_args, env) < 0)
			exit(1);
	path = get_bin_path(tokens[p->cmd_index]->str, env);
	if (execve(path, p->cmd_args, env) < 0)
		exit(1);
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

void	exec_pipeline(t_cmd **tokens, t_pipedata *p, char **env)
{
	int			status;
	int			*pids;
	int			i;
	t_pipedata	local_p;

	status = 0;
	p->pipefd = arena_malloc(sizeof(int *) * 2);
	p->pipefd[0] = arena_malloc(sizeof(int) * 2);
	p->pipefd[1] = arena_malloc(sizeof(int) * 2);
	pipe(p->pipefd[0]);
	pipe(p->pipefd[1]);
	pids = arena_malloc(sizeof(int) * (p->pipe_count + 1));
	i = 0;
	while (i < p->pipe_count + 1)
	{
		reset_sig();
		pids[i] = fork();
		if (pids[i] == 0)
		{
			local_p = *p;
			local_p.index = p->index;
			local_p.cmd_index = p->index;
			local_p.pipe_index = i;
			check_redirects_pipe(tokens, &local_p);
			setup_cmd_to_execute(tokens, &local_p);
			child_process(tokens, &local_p, env);
		}
		while (tokens[p->index] && tokens[p->index]->type != PIPE)
			p->index++;
		if (tokens[p->index] && tokens[p->index]->type == PIPE)
			p->index++;
		p->pipe_index++;
		i++;
	}
	close(p->pipefd[0][0]);
	close(p->pipefd[0][1]);
	close(p->pipefd[1][0]);
	close(p->pipefd[1][1]);
	wait_for_children(p, status, pids);
}

void	setup_pipeline(t_cmd **tokens, char **env)
{
	t_pipedata	*p;
	int			i;

	p = arena_malloc(sizeof(t_pipedata));
	p->stdout_copy = dup(STDOUT_FILENO);
	p->stdin_copy = dup(STDIN_FILENO);
	// p->outfile = dup(STDOUT_FILENO);
	// p->infile = dup(STDIN_FILENO);
	p->index = 0;
	i = 0;
	while (tokens[i])
		if (tokens[i++]->type == PIPE)
			p->pipe_count++;
	exec_pipeline(tokens, p, env);
	dup2(p->stdin_copy, STDIN_FILENO);
	dup2(p->stdout_copy, STDOUT_FILENO);
}

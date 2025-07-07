/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:20:20 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/04 15:40:15 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	safe_execve(char *path, char **argv, char **env)
{
	if (fcntl(STDOUT_FILENO, F_GETFL) == -1 && errno == EBADF)
	{
		ft_fprintf(2, "stdout pipe broken\n");
		exit(1);
	}
	return (execve(path, argv, env));
}

void	child_process(t_cmd **tokens, t_pipedata *p, char **env)
{
	char	*path;
	int		j;
	int		my_stdin;
	int		my_stdout;

	my_stdin = -1, my_stdout = -1;
	if (p->pipe_count == 0)
	{
		my_stdin = p->infile;
		my_stdout = p->outfile;
	}
	else if (p->pipe_index == 0)
	{
		my_stdin = p->infile;
		my_stdout = p->pipefd[p->pipe_index][WRITE];
	}
	else if (p->pipe_index == p->pipe_count)
	{
		my_stdin = p->pipefd[p->pipe_index - 1][READ];
		my_stdout = p->outfile;
	}
	else
	{
		my_stdin = p->pipefd[p->pipe_index - 1][READ];
		my_stdout = p->pipefd[p->pipe_index][WRITE];
	}
	j = 0;
	while (j < p->pipe_count)
	{
		if (p->pipefd[j][READ] != my_stdin)
			close(p->pipefd[j][READ]);
		if (p->pipefd[j][WRITE] != my_stdout)
			close(p->pipefd[j][WRITE]);
		j++;
	}
	dup2(my_stdin, STDIN_FILENO);
	dup2(my_stdout, STDOUT_FILENO);
	if (my_stdin != p->infile && my_stdin != STDIN_FILENO)
		close(my_stdin);
	if (my_stdout != p->outfile && my_stdout != STDOUT_FILENO)
		close(my_stdout);
	if (p->infile != STDIN_FILENO)
		close(p->infile);
	if (p->outfile != STDOUT_FILENO)
		close(p->outfile);
	if (p->is_builtin == true)
	{
		if (p->pipe_count > 0)
		{
			close(p->stdin_copy);
			close(p->stdout_copy);
			child_builds(tokens, env, p->cmd_index);
			exit(ft_atoi(find_export("?")));
		}
		build_handler(tokens);
		return ;
	}
	close(p->stdin_copy);
	close(p->stdout_copy);
	path = get_bin_path(mini_strndup(tokens[p->cmd_index]->str,
				ft_strlen(tokens[p->cmd_index]->str)), env, p);
	if (access(p->cmd_args[0], X_OK) >= 0)
		if (safe_execve(p->cmd_args[0], p->cmd_args, env) < 0)
			exit(1);
	if (safe_execve(path, p->cmd_args, env) < 0)
		exit(1);
}

static void	exec_builtin(t_cmd **tokens, t_pipedata *p, char **env)
{
	check_for_redirects(tokens, p);
	if (setup_cmd_to_execute(tokens, p) < 0)
		return ;
	child_process(tokens, p, env);
}

void	setup_child(t_cmd **tokens, t_pipedata *p, char **env, int i)
{
	t_pipedata	local_p;

	p->is_builtin = false;
	p->pids[i] = fork();
	if (p->pids[i] == 0)
	{
		local_p = *p;
		local_p.cmd_index = p->index;
		local_p.pipe_index = i;
		check_for_redirects(tokens, &local_p);
		if (setup_cmd_to_execute(tokens, &local_p) < 0)
			return ;
		child_process(tokens, &local_p, env);
	}
}

static void	exec_pipeline(t_cmd **tokens, t_pipedata *p, char **env)
{
	int	status;
	int	i;

	p->pids = arena_malloc(sizeof(int) * (p->pipe_count + 1));
	status = 0;
	i = 0;
	while (i < p->pipe_count + 1)
	{
		p->cmd_found = false;
		if (i < p->pipe_count)
			if (pipe(p->pipefd[i]) < 0)
				perror("pipe");
		reset_sig();
		if (p->pipe_count == 0 && check_for_builtin(tokens))
			exec_builtin(tokens, p, env);
		else
			setup_child(tokens, p, env, i);
		if (p->pipe_count > 0)
			find_next_cmd_index(tokens, p);
		i++;
	}
	i = 0;
	while (i < p->pipe_count)
	{
		close(p->pipefd[i][READ]);
		close(p->pipefd[i][WRITE]);
		i++;
	}
	if (p->pids[0])
		wait_for_children(p, status);
	dup2(p->stdin_copy, STDIN_FILENO);
	close(p->stdin_copy);
	dup2(p->stdout_copy, STDOUT_FILENO);
	close(p->stdout_copy);
	close(p->infile);
	close(p->outfile);
}

void	execution(t_cmd **tokens, char **env)
{
	t_pipedata	*p;
	t_data		*data;
	int			i;

	if (tokens[0]->type == HERE_DOC && tokens[1]->next == EMPTY)
		return ;
	data = get_data();
	if (data->valid != 1)
		return ;
	p = arena_malloc(sizeof(t_pipedata));
	p->index = 0;
	p->pipe_count = 0;
	p->cmd_index = 0;
	p->pipe_index = 0;
	p->infile = dup(STDIN_FILENO);
	p->outfile = dup(STDOUT_FILENO);
	p->stdin_copy = dup(STDIN_FILENO);
	p->stdout_copy = dup(STDOUT_FILENO);
	i = 0;
	while (tokens[i])
		if (tokens[i++]->type == PIPE)
			p->pipe_count++;
	if (p->pipe_count > 0)
		init_pipes(p);
	exec_pipeline(tokens, p, env);
}

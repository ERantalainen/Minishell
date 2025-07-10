/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:20:20 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/10 14:22:52 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	safe_execve(char *path, char **argv, char **env)
{
	t_stat	st;

	if (fstat(STDOUT_FILENO, &st) == -1 && errno == EBADF)
		ft_exit_child(NULL, 1);
	return (execve(path, argv, env));
}

static int	check_if_path_exists(void)
{
	char	*path;

	path = find_export("PATH");
	if (ft_strcmp(path, "") == 0)
		return (-1);
	return (1);
}

static void	setup_read_and_write_ends(t_pipedata *p, int *child_stdin,
		int *child_stdout)
{
	*child_stdin = -1;
	*child_stdout = -1;
	if (p->pipe_count == 0)
	{
		*child_stdin = p->infile;
		*child_stdout = p->outfile;
	}
	else if (p->pipe_index == 0)
	{
		*child_stdin = p->infile;
		*child_stdout = p->pipefd[p->pipe_index][WRITE];
	}
	else if (p->pipe_index == p->pipe_count)
	{
		*child_stdin = p->pipefd[p->pipe_index - 1][READ];
		*child_stdout = p->outfile;
	}
	else
	{
		*child_stdin = p->pipefd[p->pipe_index - 1][READ];
		*child_stdout = p->pipefd[p->pipe_index][WRITE];
	}
}

static void	close_unused_child_fds(t_pipedata *p, int *child_stdin,
		int *child_stdout)
{
	int	j;

	j = 0;
	while (j < p->pipe_count)
	{
		if (p->pipefd[j][READ] != *child_stdin)
			close(p->pipefd[j][READ]);
		if (p->pipefd[j][WRITE] != *child_stdout)
			close(p->pipefd[j][WRITE]);
		j++;
	}
	if (*child_stdin != p->infile && *child_stdin != STDIN_FILENO)
		close(*child_stdin);
	if (*child_stdout != p->outfile && *child_stdout != STDOUT_FILENO)
		close(*child_stdout);
	if (p->infile != STDIN_FILENO)
		close(p->infile);
	if (p->outfile != STDOUT_FILENO)
		close(p->outfile);
}

static void	execute_child_builtin(t_cmd **tokens, t_pipedata *p, char **env)
{
	if (p->pipe_count > 0)
	{
		close(p->stdin_copy);
		close(p->stdout_copy);
		child_builds(tokens, env, p->cmd_index);
		exit(ft_atoi(find_export("?")));
	}
	build_handler(tokens);
}

void	child_process(t_cmd **tokens, t_pipedata *p, char **env)
{
	char	*path;
	int		child_stdin;
	int		child_stdout;

	setup_read_and_write_ends(p, &child_stdin, &child_stdout);
	close_unused_child_fds(p, &child_stdin, &child_stdout);
	dup2(child_stdin, STDIN_FILENO);
	dup2(child_stdout, STDOUT_FILENO);
	if (p->is_builtin == true)
	{
		execute_child_builtin(tokens, p, env);
		return ;
	}
	close(p->stdin_copy);
	close(p->stdout_copy);
	path = get_bin_path(mini_strndup(tokens[p->cmd_index]->str,
				ft_strlen(tokens[p->cmd_index]->str)), env, p);
	if (access(p->cmd_args[0], X_OK) >= 0 && ft_strncmp(p->cmd_args[0], "/",
			1 == 0))
		if (safe_execve(p->cmd_args[0], p->cmd_args, env) < 0)
			ft_exit_child(NULL, 1);
	if (safe_execve(path, p->cmd_args, env) < 0 && check_if_path_exists() == 1)
		ft_exit_child(NULL, 1);
	else
		ft_exit_child(NULL, 0);
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
			ft_exit_child(NULL, 1);
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
		if (i > 0 && p->pipe_count > 0)
			close_unused_pipes(p, i);
		i++;
	}
	if (p->pids[0])
		wait_for_children(p, status);
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

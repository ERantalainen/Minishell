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

static void	check_for_redirects(t_cmd **tokens, t_pipedata *p)
{
	while (tokens[p->index])
	{
		if (tokens[p->index]->type == PIPE)
			return ;
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

static void	wait_for_children(t_pipedata *p, int status)
{
	int	i;

	i = 0;
	while (i < p->pipe_count + 1)
	{
		ignore();
		if (waitpid(p->pids[i], &status, 0) < 0)
			exit(100);
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

static int	setup_cmd_to_execute(t_cmd **tokens, t_pipedata *p)
{
	char	**split;
	size_t	total_args;
	size_t	arg_i;
	size_t	tok_i;

	while (tokens[p->cmd_index] && tokens[p->cmd_index]->type != STRING
		&& tokens[p->cmd_index]->type != BUILTIN)
		p->cmd_index++;
	if (tokens[p->cmd_index]->type == BUILTIN)
		p->is_builtin = true;
	if (!tokens[p->cmd_index])
		return (-1);
	split = mini_split(tokens[p->cmd_index]->str, ' ');
	total_args = p->cmd_index + 1;
	while (tokens[total_args] && tokens[total_args]->type == FILES)
		total_args++;
	arg_i = 0;
	while (split[arg_i++])
		total_args++;
	p->cmd_args = arena_malloc((total_args - p->cmd_index) * sizeof(char *));
	arg_i = 0;
	tok_i = 0;
	while (split[tok_i])
		p->cmd_args[arg_i++] = mini_strdup(split[tok_i++]);
	tok_i = p->cmd_index;
	if (tokens[tok_i]->next == FILES)
		tok_i++;
	while (tokens[tok_i] && tokens[tok_i]->type == FILES)
		p->cmd_args[arg_i++] = mini_strdup(tokens[tok_i++]->str);
	p->cmd_args[arg_i] = NULL;
	return (0);
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

static void	open_handler(t_pipedata *p, const char *path)
{
	int	fd;

	fd = open(p->cmd_args[0], O_RDONLY);
	if (fd < 0)
	{
		if (errno == EISDIR)
			ft_fprintf(2, "%s: Is a directory\n", p->cmd_args[0]);
		else if (errno == ENOTDIR)
			ft_fprintf(2, "%s: Not a directory\n", p->cmd_args[0]);
		else if (errno == EACCES)
			ft_fprintf(2, "%s: Permission denied\n", p->cmd_args[0]);
		else if (errno == ENOENT && access(path, X_OK) < 0
			&& (ft_strchr(p->cmd_args[0], '/') || ft_strchr(p->cmd_args[0],
					'\\')))
		{
			ft_fprintf(2, "%s: No such file or directory\n", p->cmd_args[0]);
			exit(1);
		}
	}
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
		setup_pipes(p->pipefd[p->pipe_index - 1][READ],
			p->pipefd[p->pipe_index][WRITE], 0, 0);
	if (p->pipe_count > 0)
		close_unused_pipes(p, p->pipe_index);
	if (p->is_builtin == true)
	{
		build_handler(tokens);
		if (p->pipe_count > 0)
			exit(1);
		return ;
	}
	path = get_bin_path(tokens[p->cmd_index]->str, env);
	ft_fprintf(2, "cmd: %s\n", p->cmd_args[0]);
	open_handler(p, path);
	if (access(p->cmd_args[0], X_OK) >= 0)
		if (execve(p->cmd_args[0], p->cmd_args, env) < 0)
			exit(1);
	if (execve(path, p->cmd_args, env) < 0)
		exit(1);
}

static void	setup_child(t_cmd **tokens, t_pipedata *p, char **env, int i)
{
	t_pipedata	*local_p;

	p->is_builtin = false;
	p->pids[i] = fork();
	if (p->pids[i] == 0)
	{
		local_p = p;
		local_p->cmd_index = p->index;
		check_for_redirects(tokens, local_p);
		if (setup_cmd_to_execute(tokens, local_p) < 0)
			return ;
		child_process(tokens, local_p, env);
	}
}

static void	exec_builtin(t_cmd **tokens, t_pipedata *p, char **env)
{
	check_for_redirects(tokens, p);
	if (setup_cmd_to_execute(tokens, p) < 0)
		return ;
	child_process(tokens, p, env);
}

static bool	check_for_builtin(t_cmd **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->type == BUILTIN)
			return (true);
		i++;
	}
	return (false);
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
	int	i;

	p->pids = arena_malloc(sizeof(int) * (p->pipe_count + 1));
	status = 0;
	i = 0;
	while (i < p->pipe_count + 1)
	{
		if (i < p->pipe_count)
			if (pipe(p->pipefd[i]) < 0)
				perror("pipe");
		reset_sig();
		if (p->pipe_count == 0 && check_for_builtin(tokens))
			exec_builtin(tokens, p, env);
		else
			setup_child(tokens, p, env, i);
		if (p->pipe_count > 0)
		{
			find_next_cmd_index(tokens, p);
			close_unused_pipes(p, i);
		}
		p->pipe_index++;
		i++;
	}
	if (p->pids[0])
		wait_for_children(p, status);
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
	if (p->pipe_count > 0)
		init_pipes(p);
	exec_pipeline(tokens, p, env);
}

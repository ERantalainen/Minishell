/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 13:18:10 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/17 18:34:43 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	open_handler(t_pipex *p, char *av)
{
	p->fd_cmd = open(av, O_RDONLY);
	if (p->fd_cmd < 0)
	{
		if (errno == EISDIR)
			ft_printf(STDERR_FILENO, "%s: Is a directory\n", av);
		else if (errno == ENOTDIR)
			ft_printf(STDERR_FILENO, "%s: Not a directory\n", av);
		else if (errno == EACCES)
			ft_printf(STDERR_FILENO, "%s: Permission denied\n", av);
		else if (errno == ENOENT && (ft_strchr(av, '/') || ft_strchr(av, '\\')))
		{
			ft_printf(STDERR_FILENO, "%s: No such file or directory\n", av);
			exit_handler(1, NULL, p);
		}
	}
}

static void	pipe_cleanup(t_pipex *p)
{
	int	i;

	i = 0;
	while (i < p->pipe_count)
	{
		if (close(p->pipefd[i][READ]) < 0)
			exit_handler(errno, "close failed", p);
		if (close(p->pipefd[i][WRITE]) < 0)
			exit_handler(errno, "close failed", p);
		i++;
	}
}

static void	setup_pipes(t_pipex *p)
{
	if (p->pipe_index == 0)
	{
		if (p->infile != -1 && dup2(p->infile, STDIN_FILENO) < 0)
			exit_handler(errno, "dup2 (stdin) failed", p);
		if (dup2(p->pipefd[p->pipe_index][WRITE], STDOUT_FILENO) < 0)
			exit_handler(errno, "dup2 (stdout) failed", p);
	}
	else if (p->pipe_index == p->pipe_count)
	{
		if (dup2(p->pipefd[p->pipe_index - 1][READ], STDIN_FILENO) < 0)
			exit_handler(errno, "dup2 (stdin) failed", p);
		if (p->outfile != -1 && dup2(p->outfile, STDOUT_FILENO) < 0)
			exit_handler(errno, "dup2 (stdout) failed", p);
	}
	else
	{
		if (dup2(p->pipefd[p->pipe_index - 1][READ], STDIN_FILENO) < 0)
			exit_handler(errno, "dup2 (stdin) failed", p);
		if (dup2(p->pipefd[p->pipe_index][WRITE], STDOUT_FILENO) < 0)
			exit_handler(errno, "dup2 (stdout) failed", p);
	}
}

void	child_process(t_pipex *p, char *av, char **env)
{
	setup_pipes(p);
	pipe_cleanup(p);
	open_handler(p, av);
	get_bin_path(p, av, env);
	if (!p->path)
		exit_handler(127, av, p);
	get_cmd_args(p, av);
	if (execve(p->path, p->cmd_args, env) < 0)
		exit_handler(errno, "execve failed", p);
	exit(errno);
}

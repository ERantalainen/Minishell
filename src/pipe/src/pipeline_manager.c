/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 13:18:36 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/17 18:34:34 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	wait_for_children(t_pipex *p)
{
	int	i;

	i = 0;
	while (i < p->cmd_count)
	{
		if (waitpid(p->pid[i], &p->status, 0) < 0)
			exit_handler(errno, "waitpid failed", p);
		i++;
	}
}

static void	cleanup_pipes(t_pipex *p)
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

static void	create_pipes(t_pipex *p)
{
	int	i;

	p->pipefd = ft_calloc(p->pipe_count, sizeof(int *));
	if (!p->pipefd)
		exit_handler(errno, "pipe allocation failed", p);
	i = 0;
	while (i < p->pipe_count)
	{
		p->pipefd[i] = ft_calloc(2, sizeof(int));
		if (!p->pipefd[i])
			exit_handler(errno, "pipe allocation failed", p);
		i++;
	}
	i = 0;
	while (i < p->pipe_count)
	{
		if (pipe(p->pipefd[i]) < 0)
			exit_handler(errno, "pipe creation failed", p);
		i++;
	}
}

static void	spawn_child_processes(t_pipex *p, char **av, char **env)
{
	int	i;

	i = 0;
	while (i < p->cmd_count)
	{
		p->pipe_index = i;
		p->pid[i] = fork();
		if (p->pid[i] < 0)
			exit_handler(errno, "fork failed", p);
		if (p->pid[i] == 0 && p->is_heredoc == false)
			child_process(p, av[i + 2], env);
		else if (p->pid[i] == 0 && p->is_heredoc == true)
			child_process(p, av[i + 3], env);
		i++;
	}
}

void	execute_pipeline(t_pipex *p, int ac, char **av, char **env)
{
	p->cmd_count = ac - 3;
	if (p->is_heredoc == true)
		p->cmd_count = ac - 4;
	if (p->cmd_count > PIPELINE_LIMIT)
		exit_handler(errno, "Error: too many pipes...", p);
	p->pipe_count = p->cmd_count - 1;
	p->pid = ft_calloc(p->cmd_count, sizeof(int));
	if (!p->pid)
		exit_handler(errno, "pid allocation failed", p);
	create_pipes(p);
	spawn_child_processes(p, av, env);
	cleanup_pipes(p);
	wait_for_children(p);
}

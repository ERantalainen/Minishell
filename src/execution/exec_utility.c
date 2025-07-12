/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utility.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:15:46 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/13 00:38:20 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_for_builtin(t_cmd **tokens, int pipe_count)
{
	int	i;

	i = 0;
	if (tokens[i]->type != BUILTIN && pipe_count == 0)
		return (false);
	while (tokens[i])
	{
		if (tokens[i]->type == BUILTIN)
			return (true);
		i++;
	}
	return (false);
}

void	find_next_cmd_index(t_cmd **tokens, t_pipedata *p)
{
	while (tokens[p->index] && tokens[p->index]->type != PIPE)
		p->index++;
	while (tokens[p->index] && tokens[p->index]->type == PIPE)
		p->index++;
}

void	wait_for_children(t_pipedata *p)
{
	int	i;
	int	status;

	status = 0;
	i = 0;
	while (i < p->pipe_count + 1)
	{
		ignore();
		if (waitpid(p->pids[i], &status, 0) < 0)
			ft_exit_child("waitpid", 1);
		child_died(status);
		catcher();
		i++;
	}
	if (dup2(p->stdin_copy, STDIN_FILENO) < 0)
		perror("dup2");
	safe_close(p->stdin_copy);
	if (dup2(p->stdout_copy, STDOUT_FILENO) < 0)
		perror("dup2");
	safe_close(p->stdout_copy);
	safe_close(p->infile);
	safe_close(p->outfile);
}

void	close_unused_pipes(t_pipedata *p, int i)
{
	close(p->pipefd[i - 1][READ]);
	close(p->pipefd[i - 1][WRITE]);
}

void	init_pipes(t_pipedata *p)
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

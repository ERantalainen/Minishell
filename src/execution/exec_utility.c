/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utility.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:15:46 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/22 21:52:40 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_for_builtin(t_cmd **tokens)
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

	i = -1;
	if (p->pids[0])
	{
		while (++i < p->pipe_count + 1)
		{
			if (waitpid(p->pids[i], &status, 0) < 0)
				if (errno != EINTR)
					ft_exit_child(p, "waitpid", 1);
			child_died(status);
		}
	}
	if (p->is_builtin == true && p->pipe_count == 0)
	{
		dup2(p->stdin_copy, STDIN_FILENO);
		dup2(p->stdout_copy, STDOUT_FILENO);
	}
	safe_close(&p->stdin_copy);
	safe_close(&p->stdout_copy);
	if (p->is_builtin == true && p->pipe_count == 0)
		return ;
	safe_close(&p->infile);
	safe_close(&p->outfile);
}

void	close_unused_pipes(t_pipedata *p, int i)
{
	safe_close(&p->pipefd[i - 1][READ]);
	safe_close(&p->pipefd[i - 1][WRITE]);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utility.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:15:46 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/02 21:15:47 by jpelline         ###   ########.fr       */
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

void	wait_for_children(t_pipedata *p, int status)
{
	int	i;

	i = 0;
	while (i < p->pipe_count + 1)
	{
		ignore();
		if (waitpid(p->pids[i], &status, 0) < 0)
			exit(1);
		child_died(status);
		catcher();
		i++;
	}
}

void close_unused_pipes(t_pipedata *p, int index)
{
    int i;
    
    i = 0;
    while (i < p->pipe_count)
    {
        // Close read end if this process shouldn't read from this pipe
        if (i != index - 1)
            close(p->pipefd[i][READ]);
            
        // Close write end if this process shouldn't write to this pipe  
        if (i != index)
            close(p->pipefd[i][WRITE]);
            
        i++;
    }
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

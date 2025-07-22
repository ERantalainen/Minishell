/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:20:20 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/22 13:45:09 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_child(t_cmd **tokens, t_pipedata *p, char **env, int i)
{
	t_pipedata	local_p;

	p->is_child = true;
	p->is_builtin = false;
	p->has_out_redirect = false;
	reset_sig();
	p->pids[i] = fork();
	if (p->pids[i] < 0)
	{
		handle_failure(p, "fork");
		return (-1);
	}
	if (p->pids[i] == 0)
	{
		local_p = *p;
		local_p.cmd_index = p->index;
		local_p.pipe_index = i;
		if (check_for_redirects(tokens, &local_p) < 0)
			ft_exit_child(p, NULL, 1);
		if (setup_cmd_to_execute(tokens, &local_p) < 0)
			ft_exit_child(p, NULL, 0);
		child_process(tokens, &local_p, env);
	}
	ignore();
	return (0);
}

static void	exec_builtin(t_cmd **tokens, t_pipedata *p, char **env)
{
	p->is_child = false;
	p->is_builtin = true;
	if (check_for_redirects(tokens, p) < 0)
		return ;
	if (setup_cmd_to_execute(tokens, p) < 0)
		return ;
	child_process(tokens, p, env);
}

void	close_pipe_pair(t_pipedata *p, int i)
{
	safe_close(&p->pipefd[i][READ]);
	safe_close(&p->pipefd[i][WRITE]);
}

static void	exec_pipeline(t_cmd **tokens, t_pipedata *p, char **env)
{
	int	i;

	p->pids = arena_malloc(sizeof(int) * (p->pipe_count + 1));
	i = 0;
	while (i < p->pipe_count + 1)
	{
		p->cmd_found = false;
		if (i < p->pipe_count && pipe(p->pipefd[i]) < 0)
			return (handle_failure(p, "pipe"));
		if (p->pipe_count == 0 && check_for_builtin(tokens, p->pipe_count))
			exec_builtin(tokens, p, env);
		else if (setup_child(tokens, p, env, i) < 0)
			return ;
		if (p->pipe_count > 0)
			find_next_cmd_index(tokens, p);
		if (i > 0)
			close_pipe_pair(p, i - 1);
		i++;
	}
	wait_for_children(p);
}

void	execution(t_cmd **tokens, char **env)
{
	t_pipedata	*p;
	t_data		*data;
	int			i;

	if (!only_heredocs(tokens) || !only_empty_export(tokens))
		return ;
	data = get_data();
	if (data->valid != 1)
		return ;
	close(data->stdin);
	p = arena_malloc(sizeof(t_pipedata));
	if (init_pipedata(p) < 0)
		return ;
	i = 0;
	while (tokens[i])
		if (tokens[i++]->type == PIPE)
			p->pipe_count++;
	if (p->pipe_count > 0)
		init_pipes(p);
	exec_pipeline(tokens, p, env);
	data->stdin = dup(STDIN_FILENO);
	catcher();
}

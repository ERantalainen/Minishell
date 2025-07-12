/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:20:20 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/12 23:43:54 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_child(t_cmd **tokens, t_pipedata *p, char **env, int i)
{
	t_pipedata	local_p;

	p->is_builtin = false;
	p->pids[i] = fork();
	if (p->pids[i] < 0)
		ft_exit_child("fork", 1);
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

static void	exec_builtin(t_cmd **tokens, t_pipedata *p, char **env)
{
	check_for_redirects(tokens, p);
	if (setup_cmd_to_execute(tokens, p) < 0)
		return ;
	child_process(tokens, p, env);
}

static void	close_pipe_pair(t_pipedata *p, int i)
{
	safe_close(p->pipefd[i][READ]);
	safe_close(p->pipefd[i][WRITE]);
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
			perror("pipe");
		reset_sig();
		if (p->pipe_count == 0 && check_for_builtin(tokens, p->pipe_count))
			exec_builtin(tokens, p, env);
		else
			setup_child(tokens, p, env, i);
		if (p->pipe_count > 0)
			find_next_cmd_index(tokens, p);
		if (i > 0)
			close_pipe_pair(p, i - 1);
		i++;
	}
	if (p->pids[0])
		wait_for_children(p);
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
	if (init_pipdata(p) < 0)
		return ;
	i = 0;
	while (tokens[i])
		if (tokens[i++]->type == PIPE)
			p->pipe_count++;
	if (p->pipe_count > 0)
		init_pipes(p);
	exec_pipeline(tokens, p, env);
}

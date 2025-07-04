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

int	setup_cmd_to_execute(t_cmd **tokens, t_pipedata *p)
{
	char	**split;
	size_t	total_args;
	size_t	arg_i;
	size_t	tok_i;

	while (tokens[p->cmd_index]
		&& tokens[p->cmd_index]->type != STRING
		&& tokens[p->cmd_index]->type != FILES
		&& tokens[p->cmd_index]->type != BUILTIN)
		p->cmd_index++;
	if (tokens[p->cmd_index]
		&& tokens[p->cmd_index]->type == FILES
		&& tokens[p->cmd_index]->next == FILES)
		tokens[p->cmd_index + 1]->type = STRING;
	while (tokens[p->cmd_index]
		&& tokens[p->cmd_index]->type != STRING
		&& tokens[p->cmd_index]->type != PIPE
		&& tokens[p->cmd_index]->type != BUILTIN)
		p->cmd_index++;
	if (tokens[p->cmd_index] && tokens[p->cmd_index]->type == BUILTIN)
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
	p->cmd_found = true;
	return (0);
}

void	setup_pipes(int in, int out, int close_in, int close_out)
{
	if (dup2(in, STDIN_FILENO) < 0)
		ft_exit_child(NULL, 1);
	if (close_in && in != STDIN_FILENO)
		close(in);
	if (dup2(out, STDOUT_FILENO) < 0)
		ft_exit_child(NULL, 1);
	if (close_out && out != STDOUT_FILENO)
		close(out);
}

void	child_process(t_cmd **tokens, t_pipedata *p, char **env)
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
		if (p->pipe_count > 0)
		{
			child_builds(tokens, env, p->cmd_index);
			exit(ft_atoi(find_export("?")));
		}
		build_handler(tokens);
		return ;
	}
	path = get_bin_path(mini_strndup(tokens[p->cmd_index]->str,
				ft_strlen(tokens[p->cmd_index]->str)), env, p);
	// open_handler(p, path);
	if (access(p->cmd_args[0], X_OK) >= 0)
		if (execve(p->cmd_args[0], p->cmd_args, env) < 0)
			exit(1);
	if (execve(path, p->cmd_args, env) < 0)
		exit(1);
}

static void	exec_builtin(t_cmd **tokens, t_pipedata *p, char **env)
{
	check_for_redirects(tokens, p);
	if (setup_cmd_to_execute(tokens, p) < 0)
		return ;
	child_process(tokens, p, env);
}

static void	close_all_pipes(t_pipedata *p)
{
	int	i;

	i = 0;
	while (i < p->pipe_count)
	{
		close(p->pipefd[i][READ]);
		close(p->pipefd[i][WRITE]);
		i++;
	}
	if (p->infile != STDIN_FILENO)
		close(p->infile);
	if (p->outfile != STDOUT_FILENO)
		close(p->outfile);
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
		{
			find_next_cmd_index(tokens, p);
			close_unused_pipes(p, p->pipe_index);
		}
		p->pipe_index++;
		i++;
	}
	close_all_pipes(p);
	if (p->pids[0])
		wait_for_children(p, status);
}

void	execution(t_cmd **tokens, char **env)
{
	t_pipedata	*p;
	t_data		*data;
	int			i;

	
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
	i = 0;
	while (tokens[i])
		if (tokens[i++]->type == PIPE)
			p->pipe_count++;
	if (p->pipe_count > 0)
		init_pipes(p);
	exec_pipeline(tokens, p, env);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 13:28:50 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/15 11:15:10 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_next_cmd_in_tokens(t_cmd **tokens, t_pipedata *p)
{
	if (tokens[p->cmd_index] && (tokens[p->cmd_index]->type == INPUT
			|| tokens[p->cmd_index]->type == OUTPUT
			|| tokens[p->cmd_index]->type == APPEND)
		&& tokens[p->cmd_index]->next == BUILTIN)
		p->cmd_index += 2;
	while (tokens[p->cmd_index] && tokens[p->cmd_index]->type != STRING
		&& tokens[p->cmd_index]->type != FILES
		&& tokens[p->cmd_index]->type != BUILTIN)
		p->cmd_index++;
	if (tokens[p->cmd_index] && tokens[p->cmd_index]->type == FILES
		&& tokens[p->cmd_index]->next == FILES)
		tokens[p->cmd_index + 1]->type = STRING;
	while (tokens[p->cmd_index] && tokens[p->cmd_index]->type != STRING
		&& tokens[p->cmd_index]->type != PIPE
		&& tokens[p->cmd_index]->type != BUILTIN)
		p->cmd_index++;
	if (tokens[p->cmd_index] && tokens[p->cmd_index]->type == BUILTIN)
		p->is_builtin = true;
	if (!tokens[p->cmd_index] && p->pipe_count == 0)
		return (-1);
	else if ((!tokens[p->cmd_index] || tokens[p->cmd_index]->type == PIPE)
		&& p->pipe_count > 0)
		ft_exit_child(p, NULL, 1);
	return (0);
}

static size_t	get_cmd_array_size(t_cmd **tokens, t_pipedata *p, char **split)
{
	size_t	total_args;
	size_t	arg_i;

	total_args = 0;
	arg_i = 0;
	total_args = p->cmd_index + 1;
	while (tokens[total_args] && (tokens[total_args]->type == FILES
			|| tokens[total_args]->type == STRING
			|| tokens[total_args]->type == BUILTIN
			|| tokens[total_args]->type == HERE_DOC))
		total_args++;
	arg_i = 0;
	while (split[arg_i++])
		total_args++;
	return (total_args);
}

static void	check_and_add_arguments(t_cmd **tokens, t_pipedata *p,
	size_t *arg_i,
		size_t *tok_i)
{
	if (*tok_i != p->cmd_index)
	{
		p->cmd_args[*arg_i] = "";
		while (tokens[*tok_i] && (tokens[*tok_i]->type == FILES
				|| tokens[*tok_i]->type == STRING
				|| tokens[*tok_i]->type == BUILTIN))
		{
			if (ft_strcmp(tokens[*tok_i]->str, "") != 0 || (tokens[*tok_i + 1]
					&& tokens[*tok_i + 1]->space))
			{
				if (tokens[*tok_i]->space)
					p->cmd_args[*arg_i] = mini_strdup(tokens[*tok_i]->str);
				else
					p->cmd_args[*arg_i] = mini_join(p->cmd_args[*arg_i],
							tokens[*tok_i]->str);
			}
			if ((tokens[*tok_i + 1] && tokens[*tok_i + 1]->space)
				|| (tokens[*tok_i]->next != FILES
					&& tokens[*tok_i]->next != STRING
					&& tokens[*tok_i]->next != BUILTIN))
				(*arg_i)++;
			(*tok_i)++;
			*tok_i = skip_redirects(tokens, *tok_i);
		}
	}
}

static void	additional_arguments_to_cmd(t_cmd **tokens, t_pipedata *p,
		size_t arg_i, size_t tok_i)
{
	tok_i = p->cmd_index;
	if (tokens[tok_i] && (tokens[tok_i]->next == FILES
			|| tokens[tok_i]->next == HERE_DOC
			|| tokens[tok_i]->next == BUILTIN
			|| tokens[tok_i]->next == STRING))
		tok_i++;
	if (tokens[tok_i] && tokens[tok_i]->type == HERE_DOC)
		tok_i++;
	check_and_add_arguments(tokens, p, &arg_i, &tok_i);
	p->cmd_args[arg_i] = NULL;
}

int	setup_cmd_to_execute(t_cmd **tokens, t_pipedata *p)
{
	char	**split;
	size_t	total_args;
	size_t	arg_i;
	size_t	tok_i;

	if (find_next_cmd_in_tokens(tokens, p) < 0)
		return (-1);
	split = mini_split(tokens[p->cmd_index]->str, ' ');
	total_args = get_cmd_array_size(tokens, p, split);
	p->cmd_args = arena_malloc((total_args - p->cmd_index) * sizeof(char *));
	arg_i = 0;
	tok_i = 0;
	while (split[tok_i])
		p->cmd_args[arg_i++] = mini_strdup(split[tok_i++]);
	additional_arguments_to_cmd(tokens, p, tok_i, arg_i);
	p->cmd_found = true;
	return (0);
}

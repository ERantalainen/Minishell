/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 13:28:50 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/07 13:29:04 by jpelline         ###   ########.fr       */
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
	if (!tokens[p->cmd_index] && p->pipe_count == 0)
		return (-1);
	else if (!tokens[p->cmd_index] && p->pipe_count > 0)
		exit(1);
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

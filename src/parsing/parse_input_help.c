/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:39:57 by erantala          #+#    #+#             */
/*   Updated: 2025/07/08 22:48:53 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*make_cmd_str(t_vector *tokens, size_t *i, t_data *data)
{
	t_token	*tk;
	t_cmd	*cmd;

	cmd = arena_malloc(sizeof(t_cmd));
	tk = tokens->data[(*i)];
	cmd->type = STRING;
	cmd->str = "";
	cmd->space = tk->space;
	cmd->quoted = tk->quoted;
	if (data->last != FILES && ((((*i == 0) || ((access(tk->s, R_OK | W_OK) != 0
		&& tk->space == 1)) || data->last == PIPE)) || tk->quoted == 1))
		cmd_help(tokens, i, tk, cmd);
	else
	{
		cmd->type = FILES;
		cmd->str = tk->s;
		(*i)++;
	}
	if ((*i) < tokens->count)
		cmd->next = tk->t;
	else
		cmd->next = EMPTY;
	built_in(cmd);
	data->last = cmd->type;
	return (cmd);
}

// Make a command of type FILES or STRING

t_cmd	*make_cmd_spc(t_vector *tokens, size_t *i, t_data *data)
{
	t_token	*token;
	t_cmd	*cmd;

	cmd = arena_malloc(sizeof(t_cmd));
	token = tokens->data[(*i)];
	cmd->type = token->t;
	cmd->space = token->space;
	cmd->quoted = token->quoted;
	cmd->str = token->s;
	(*i)++;
	if ((*i) < tokens->count)
	{
		token = tokens->data[(*i)];
			cmd->next = token->t;
	}
	else
		cmd->next = EMPTY;
	data->last = cmd->type;
	return (cmd);
}

// Make a redirect or pipe command
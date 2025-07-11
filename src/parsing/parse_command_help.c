/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_help.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 02:20:24 by erantala          #+#    #+#             */
/*   Updated: 2025/07/11 17:30:17 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_trim_check(t_vector *commands)
{
	t_cmd	*cmd;
	size_t	i;

	i = 0;
	cmd = (t_cmd *)commands->data[1];
	if (!cmd || (cmd->type != STRING && cmd->type != FILES))
		return ;
	if (!cmd->quoted)
	{
		while (cmd->str[i] && ft_isspace(cmd->str[i]))
			i++;
	}
	else
		return ;
	cmd->str += i;
	commands->data[1] = cmd;
}

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
	if (cmd->type == PIPE)
		data->check_build = 1;
	return (cmd);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_help.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 02:20:24 by erantala          #+#    #+#             */
/*   Updated: 2025/07/13 04:40:28 by erantala         ###   ########.fr       */
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

static t_cmd	*export_multiword(t_data *data, t_cmd *cmd, int i)
{
	t_cmd	*extra_token;

	if (i != 0 || cmd->type != BUILTIN 
		||	ft_strlen(cmd->str) == word_len(cmd->str, 0))
		return (cmd);
	extra_token = arena_malloc(sizeof(t_cmd));
	extra_token->space = 0;
	extra_token->quoted = 0;
	extra_token->str = mini_strndup(cmd->str, word_len(cmd->str, 0));	
	extra_token->type = BUILTIN;
	cmd->str = mini_strdup(cmd->str + word_len(cmd->str, 0) + 1);
	cmd->space = 1;
	cmd->type = STRING;
	add_elem(data->cmds, extra_token);
	return (extra_token);
}

static	t_cmd	*build_help(size_t *i, t_cmd *cmd, t_data *data)
{
	built_in(cmd, *i);
	if (cmd->type == BUILTIN)
		data->check_build = 0;
	cmd = export_multiword(data, cmd, *i);
	return (cmd);
}

t_cmd	*cmd_help(t_cmd *cmd, size_t *i, t_token *token, t_data *data)
{
	bool	join;

	join = 0;
	while ((*i) < data->tokens->count && (token->t == STRING 
		|| token->t == FILES))
	{
		if (join == 0)
			cmd->str = token->s;
		else
			cmd->str = mini_join(cmd->str, token->s);
		if (data->check_build == 1)
			build_help(i, cmd, data);
		(*i)++;
		if (data->tokens->data[*i])
			token = data->tokens->data[*i];
		if (token->space == 1)
			break ;
		join = 1;
	}
	return (cmd);
}

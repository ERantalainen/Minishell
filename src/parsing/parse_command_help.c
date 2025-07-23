/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_help.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 02:20:24 by erantala          #+#    #+#             */
/*   Updated: 2025/07/23 16:39:08 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*command_checks(t_data *dt, t_token *tk, t_cmd *cmd, size_t *i)
{
	if (dt->last != FILES
		&& ((((*i == 0) || ((access(tk->s, R_OK | W_OK) != 0
							&& tk->space == 1))
					|| dt->last == PIPE)) || tk->quoted == 1))
		return (cmd_help(cmd, i, tk, dt));
	else
	{
		cmd->type = FILES;
		cmd->str = tk->s;
		(*i)++;
	}
	return (cmd);
}

static t_cmd	*export_multiword(t_data *data, t_cmd *cmd, int i)
{
	t_cmd	*extra_token;

	if (i != 0 || cmd->type != BUILTIN
		|| ft_strlen(cmd->str) == word_len(cmd->str, 0))
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
	command_checks(data, tk, cmd, i);
	if ((*i) < tokens->count)
		cmd->next = tk->t;
	else
		cmd->next = EMPTY;
	if (data->check_build == 1)
		build_help(i, cmd, data);
	data->last = cmd->type;
	return (cmd);
}

t_cmd	*cmd_help(t_cmd *cmd, size_t *i, t_token *token, t_data *data)
{
	bool	join;

	join = 0;
	while ((*i) < data->tokens->count && (token->t == STRING
			|| token->t == FILES || token->t == HERE_NOEXP))
	{
		if (join == 0)
		{
			cmd->str = token->s;
			cmd = split_multi(data, cmd, i);
		}
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
		if (token->quoted == 1)
			cmd->quoted = 1;
	}
	return (cmd);
}

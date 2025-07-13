/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:39:57 by erantala          #+#    #+#             */
/*   Updated: 2025/07/13 03:15:49 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*command_checks(t_data *data, t_token *tk, t_cmd *cmd, size_t *i)
{
	
	if (data->last != FILES
		&& ((((*i == 0) || ((access(tk->s, R_OK | W_OK) != 0 && tk->space == 1))
					|| data->last == PIPE)) || tk->quoted == 1))
		return (cmd_help(cmd, i, tk, data));
	else
	{
		cmd->type = FILES;
		cmd->str = tk->s;
		(*i)++;
	}
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
	data->last = cmd->type;
	return (cmd);
}

// Make a command of type FILES or STRING
static char	*unquoted_expan_help(char *token, size_t *pos,
	char *s, bool space)
{
	int		len;
	char	*res;
	int		i;

	i = 0;
	res = "";
	len = ft_strlen(token);
	if (ft_strcmp(token, "") == 0)
	{
		*pos += key_len(s);
		return ("");
	}
	while (token[i] && i < len)
	{
		if (!ft_isspace(token[i]) || space == 0)
			res = mini_join(res, mini_strndup(token + i, 1));
		if (ft_isspace(token[i]))
			space = 1;
		if (!ft_isspace(token[i]))
			space = 0;
		i++;
	}
	*pos += key_len(s);
	return (res);
}

char	*unquoted_expan(char *s, size_t *pos)
{
	char	*token;
	bool	space;

	if (s[0] == '$' && (s[1] == '"' || s[1] == '\''))
	{
		*pos += 1;
		return ("");
	}
	if (s[0] == '$' && !s[1])
	{
		*pos += 1;
		return ("$");
	}
	space = 0;
	token = find_export(mini_strndup(s, key_len(s)));
	return (unquoted_expan_help(token, pos, s, space));
}

t_vector	*create_commands(t_vector *tokens)
{
	t_token		*curr;
	size_t		i;
	t_data		*data;

	data = get_data();
	if (!tokens || data->valid == 0)
	{
		data->valid = 0;
		return (NULL);
	}
	i = 0;
	data->check_build = 1;
	data->cmds = new_vector(4);
	while (i < data->tokens->count)
	{
		curr = tokens->data[i];
		if (curr->t == STRING || curr->t == FILES)
			add_elem(data->cmds, make_cmd_str(tokens, &i, data));
		else
			add_elem(data->cmds, make_cmd_spc(tokens, &i, data));
	}
	next_check(data->cmds);
	first_trim_check(data->cmds);
	return (data->cmds);
}

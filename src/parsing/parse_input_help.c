/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:39:57 by erantala          #+#    #+#             */
/*   Updated: 2025/07/22 21:14:36 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*split_multi(t_data *data, t_cmd *cmd, size_t *i)
{
	t_cmd	*new;
	char	**split;
	t_token	*token;
	size_t	j;

	j = 0;
	split = mini_split(cmd->str, ' ');
	if (cmd->quoted == 1 || ft_stralen(split) < 2)
		return (cmd);
	token = data->tokens->data[*i + 1];
	if (token && cmd->str[ft_strlen(cmd->str) - 1] == ' ')
		token->space = 1;
	while (j < ft_stralen(split) - 1)
	{
		new = arena_malloc(sizeof(t_cmd));
		new->str = split[j];
		new->space = 1;
		new->type = STRING;
		add_elem(data->cmds, new);
		j++;
	}
	cmd->space = 1;
	cmd->str = split[j];
	return (cmd);
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

// Make a command of type FILES or STRING
static char	*unquoted_expan_help(char *token, size_t *pos,
	char *s, bool space)
{
	char	*res;
	int		i;

	i = 0;
	res = "";
	if (ft_strcmp(token, "") == 0)
	{
		*pos += key_len(s);
		return ("");
	}
	while (token[i])
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
		if (curr->t == STRING || curr->t == FILES || curr->t == HERE_NOEXP)
			add_elem(data->cmds, make_cmd_str(tokens, &i, data));
		else
			add_elem(data->cmds, make_cmd_spc(tokens, &i, data));
	}
	next_check(data->cmds);
	first_trim_check(data->cmds);
	return (data->cmds);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:38:10 by erantala          #+#    #+#             */
/*   Updated: 2025/07/22 14:57:26 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *s, size_t *i, t_type last, t_data *data)
{
	t_token	*new;

	data->last = EMPTY;
	new = arena_malloc(sizeof(t_token));
	new->s = token_string(s, i, &last);
	if (data->valid == 0)
		return (NULL);
	new->quoted = 0;
	new->space = 0;
	new->t = STRING;
	if ((last == QUOTED || s[*i - 1] == '"' || s[*i - 1] == '\''))
		new->quoted = 1;
	if (last == EXPANSION)
		new->expansion = 1;
	else
		new = check_type(new, data, i, s);
	if ((last == HERE_DOC && (s[(*i - 1)] == '"' || s[(*i - 1)] == '\''))
		|| (last == HERE_NOEXP && new->t == STRING))
		new->t = HERE_NOEXP;
	return (new);
}

// check for token type
t_vector	*token_vector(char *s)
{
	size_t		len;
	t_data		*data;
	t_vector	*tokens;
	size_t		i;

	data = get_data();
	i = 0;
	len = ft_strlen(s);
	tokens = new_vector(4);
	data->tokens = tokens;
	data->input = mini_strdup(s);
	tokens = creator(s, len, i, data);
	if (tokens->count == 0)
		return (NULL);
	check_repeat(tokens);
	return (tokens);
}

// Create token vector
t_vector	*creator(char *s, size_t len, size_t i, t_data *data)
{
	t_token	*token;
	bool	space;

	token = NULL;
	while (i < len && s[i] && data->valid == 1)
	{
		space = 0;
		if (ft_isspace(s[i]))
			space = 1;
		while (s[i] && ft_isspace(s[i]) == 1)
			i++;
		if (!s[i])
			break ;
		if (data->tokens->count == 0 && s[i])
			token = create_token(s, &i, EMPTY, data);
		else if (s[i])
			token = create_token(s, &i, token->t, data);
		if (data->valid == 0)
			break ;
		if (space == 1)
			token->space = 1;
		add_elem(data->tokens, token);
	}
	return (data->tokens);
}

int	check_empty_quote(char *s, size_t *i)
{
	size_t	pos;

	pos = 0;
	if (s[pos] && s[pos] == '"' && s[pos + 1] && s[pos + 1] == '"')
	{
		*i += 2;
		return (1);
	}
	if (s[pos] && s[pos] == '\'' && s[pos + 1] && s[pos + 1] == '\'')
	{
		*i += 2;
		return (1);
	}
	return (0);
}

char	*token_string(char *s, size_t *i, t_type *last)
{
	char	*token;
	int		len;

	if (check_empty_quote(s + *i, i))
		return ("");
	if ((*last == INPUT || *last == OUTPUT || *last == APPEND) && s[*i] == '$')
		if (ambigous(s, *i) != NULL)
			return (NULL);
	if (s[(*i)] == '\'' || s[(*i)] == '"'
		|| *last == HERE_DOC || *last == HERE_NOEXP)
		return (quoted_token(s + *i, s[(*i)], i, last));
	if (s[*i] == '$')
	{
		token = unquoted_expan(s + *i, i);
		*last = EXPANSION;
	}
	else
	{
		len = word_len(s + (*i), 0);
		token = mini_strndup(s + *i, len);
		(*i) += len;
	}
	return (token);
}

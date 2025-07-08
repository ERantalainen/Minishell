/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:38:10 by erantala          #+#    #+#             */
/*   Updated: 2025/07/08 19:50:40 by erantala         ###   ########.fr       */
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
	if (last == QUOTED)
		new->quoted = 1;
	if (s[*i - 1] == '"' || s[*i - 1] == '\'')
		new->t = STRING;
	else if (data->tokens->count == 0 && !ft_isspace(s[*i]))
		new->t = STRING;
	if (ft_strncmp(new->s, "|", 1) == 0)
		new->t = PIPE;
	else if (ft_strcmp(new->s, "<") == 0)
		new->t = INPUT;
	else if (ft_strcmp(new->s, ">") == 0)
		new->t = OUTPUT;
	else if (ft_strncmp(new->s, ">>", 2) == 0)
		new->t = APPEND;
	else
		new->t = STRING;
	if (ft_strncmp(new->s, "<<", 2) == 0 && (s[*i - 1] != '"' && s[*i- 1] != '\''))
		new->t = HERE_DOC;
	if (new->t == STRING && (last == HERE_DOC && (s[(*i)] == '"'
			|| s[(*i)] == '\'' || data->last == HERE_NOEXP)))
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
	check_heredoc(tokens);
	check_repeat(tokens);
	return (tokens);
}

// Create token vector

t_vector	*creator(char *s, size_t len, size_t i, t_data *data)
{
	t_token	*token;
	bool	space;

	space = 0;
	while (i < len && s[i] && data->valid == 1)
	{
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
		add_elem(data->tokens, token);
		if (space == 1)
		{
			token->space = 1;
			space = 0;
		}
	}
	return (data->tokens);
}

int	check_empty_quote(char *s)
{
	size_t	pos;

	pos = 0;
	if (s[pos] && s[pos] == '"' && s[pos + 1] && s[pos + 1] == '"')
		return (1);
	if(s[pos] && s[pos] == '\'' && s[pos + 1] && s[pos + 1] == '\'')
		return (1);
	return (0);
}

char	*token_string(char *s, size_t *i, t_type *last)
{
	char	*token;
	int		len;

	if (check_empty_quote(s + *i))
	{
		*i += 2;
		return ("");
	}
	if ((*last == INPUT || *last == OUTPUT || *last == APPEND) && s[*i] == '$')
		if (ambigous(s, *i) != NULL)
			return (NULL);
	if (s[(*i)] == '\'' || s[(*i)] == '"' || *last == HERE_DOC)
 		return (quoted_token(s + *i, s[(*i)], i, last));
	len = word_len(s + (*i), 0);
	token = expand_strndup(s + (*i), len);
	(*i) += len;
	return (token);
}

// Create a string for the token

t_vector	*create_commands(t_vector *tokens)
{
	t_vector	*commands;
	t_token		*curr;
	size_t		i;
	t_data		*data;

	data = get_data();
	if (data->valid == 0 || !tokens)
		return (NULL);
	i = 0;
	commands = new_vector(tokens->count);
	while (i < tokens->count && tokens->data[i] != NULL)
	{
		curr = tokens->data[i];
		if (curr->t == STRING || curr->t == FILES)
			add_elem(commands, make_cmd_str(tokens, &i, data));
		else
			add_elem(commands, make_cmd_spc(tokens, &i, data));
	}
	next_check(commands);
	return (commands);
}

// Combine tokens into commands.

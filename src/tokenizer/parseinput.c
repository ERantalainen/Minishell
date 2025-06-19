/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseinput.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:38:10 by erantala          #+#    #+#             */
/*   Updated: 2025/06/19 19:31:03 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ADJUST ADJUST ADJUST ADJUST ADJUST WHICH FUNCTION IS BEING CALLED AND WHEN !!!!
// https://www.gnu.org/software/bash/manual/bash.html#Shell-Syntax

t_token	*create_token(char *s, int *i)
{
	t_token	*new;

	new = arena_malloc(sizeof(t_token));
	new->s = token_string(s, i);
	if (ft_strcmp(new->s, "|") == 0)
		new->t = PIPE;
	else if (ft_strcmp(new->s, "<") == 0)
		new->t = INPUT;
	else if (ft_strcmp(new->s, ">") == 0)
		new->t = OUTPUT;
	else if (ft_strcmp(new->s, "<<") == 0)
		new->t = HERE_DOC;
	else if (ft_strcmp(new->s, ">>") == 0)
		new->t = APPEND;
	else
		new->t = STRING;
	return (new);
}

t_vector	*token_vector(char *s)
{
	int	i;
	t_vector	*tokens;
	t_token		*token;

	tokens = new_vector(4);
	i = 0;
	while (s[i])
	{
		token = create_token(s, &i);
		add_elem(tokens, token);
		while (ft_isspace(s[i]) == 1 && s[i])
			i++;
	}
	return (tokens);
}
char	*token_string(char	*s, int	*i)
{
	char	*token;
	int		len;

	if (s[(*i)] == '\'' || s[(*i)] == '"')
	{
		return (quoted_token(s, s[(*i)], i));
	}
	len = word_len(s);
	token = mini_strndup(s, len, 1);
	(*i) += len + 1;
	return (token);
}

t_vector	*create_commands(t_vector *tokens)
{
	t_vector	*commands;
	int	i;
	int	j;

	j = 0;
	i = 0;
	commands = new_vector(tokens->count);
	while (i < tokens->count)
	{
		commands->data[j] = make_command(tokens, &i);
		j++;
	}
	return (commands);
}

t_cmd	*make_command(t_vector *tokens, int *i)
{
	t_token	*token;
	t_cmd	*command;

	command = arena_malloc(sizeof(command));
	token = tokens->data[*i];
	command->type = token->t;
	while (token->t == STRING)
	{
		mini_append(command->str, token->s);
		(*i)++;
		token = tokens->data[*i];
	}
	if (command->str != 0)
		command->next = token->t;
	else
	{
		command->str = token->s;
		token = tokens->data[++(*i)];
		command->next = token->t;
	}
	return (command);
}

void	mini_append(char *s1, char *s2)
{
	char	*dup;

	dup = arena_malloc(ft_strlen(s1) && ft_strlen(s2) + 1);
	ft_strlcat(dup, s1, ft_strlen(s1));
	ft_strlcat(dup, s2, ft_strlen(dup) + ft_strlen(s2) + 1);
	s1 = dup;
}

char	*quoted_token(char *s, char quote, int *i)
{
	char	*str;
	int		pos;

	pos = 0;
	if (quote == '\'')
	{
		while (s[pos] != '\'' && s[pos])
			pos++;
		str = mini_strndup(s + *i, pos);
		(*i) += pos;
		return (str);
	}
	else
	{
		while (s[pos] != '"' && s[pos])
			pos++;
		str = mini_strndup(s + *i, pos);
		(*i) += pos;
		return (str);
	}
}

// Creates a token with quotes

char	*mini_strndup(char *s, size_t n)
{
	char	*dup;
	size_t	i;

	i = 0;
	dup = arena_malloc(sizeof(n + 1));
	while (i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

// Simply makes a duplicate string with size N not considering expansions.

size_t	word_len(char *s)
{
	int	i;

	i = 0;
	while (s[i] && ft_isspace(s[i]) == 0)
		i++;
	return (i);
}

// Counts the length of a single word.

char	*expand_strndup(char *s, size_t n)
{
	char	*dup;
	char	*expansion;
	size_t	len;
	size_t	i;
	size_t	pos;

	pos = 0;
	i = 0;
	len = expanded_length(s, n);
	dup = arena_malloc(sizeof(len + 1));
	while (i < len)
	{
		if (s[i] == '$')
		{
			expansion = find_export(s + i);
			i += word_len(s + i);
			ft_strlcat(dup, expansion, ft_strlen(expansion) + pos);
			pos += ft_strlen(expansion);
			if (i >= len)
				break ;
		}
		dup[pos++] = s[i++];
	}
	dup[pos] = '\0';
	return (dup);
}

// Get the total length of dup once expanded. If an expansion ($) is detected
// Find the expansion, cat it onto the new string and adjust positions.
// Null terminate and return the result

size_t	expanded_length(char *s, size_t n)
{
	size_t	i;
	size_t	total;

	i = 0;
	total = 0;
	while (s[i] && i <  n)
	{
		if (s[i] == '$')
		{
			total += ft_strlen(find_export(s + i));
			total -= word_len(s + i);
		}
		i++;
	}
	return (i + total);
}

// Get total length of a token with expansions.
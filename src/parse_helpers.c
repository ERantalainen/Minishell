/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:43:47 by erantala          #+#    #+#             */
/*   Updated: 2025/07/01 15:02:28 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	word_len(char *s)
{
	int		i;
	char	c;

	i = 0;
	while (s[i] && ft_isspace(s[i]) == 0 && !check_specials(s[i]))
		i++;
	if (i == 0 && check_specials(s[i]))
	{
		if (s[i] == '|')
			return (1);
		c = s[i];
		if (s[i + 1] && s[i + 1] != c)
			return (1);
		else
			return (2);
	}
	return (i);
}

// Count the lenght of a word

int	check_specials(int	c)
{
	if (c == '<' || c == '|' || c == '>' || c == '\'' || c == '"')
		return (1);
	return (0);
}

// Check for redirect or pipe character

// Counts the length of a single word.

char	*expand_strndup(char *s, size_t n)
{
	char	*dup;
	size_t	len;
	size_t	i;
	size_t	pos;

	pos = 0;
	i = 0;
	len = expanded_length(s, n);
	dup = arena_malloc(len + 1);
	while (s[i] && i < n)
	{
		if (s[i] == '$' && s[i + 1])
			expans_help(s, dup, &i, &pos);
		if (i >= n)
			break ;
		if (s[i] == '"' || s[i] == '\'')
			i++;
		dup[pos++] = s[i++];
	}
	dup[pos] = '\0';
	return (dup);
}

char	*expans_help(char *s, char *dup, size_t *i, size_t *pos)
{
	char	*expansion;

	expansion = find_export(mini_strndup((s + (*i) + 1), quote_len(s + (*i) + 1, '"')));
	if (ft_strcmp(expansion, "") == 0 && (s[(*i) + 1] == '\'' || s[(*i) + 1] == '"'))
	(*i)++;
	else
	{
		(*i) += word_len(s + (*i));
		ft_strlcat(dup, expansion, ft_strlen(expansion) + 1 + (*pos));
		(*pos) += ft_strlen(expansion);
	}
	return (expansion);
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
			total += ft_strlen(find_export(mini_strndup(s + i + 1, quote_len(s + i, '"'))));
			total -= word_len(s + i);
		}
		i++;
	}
	return (i + total);
}

// Get total length of a token with expansions.

char	*mini_strndup(char *s, size_t n)
{
	char	*dup;
	size_t	i;

	i = 0;
	dup = arena_malloc((n + 1) * sizeof(char));
	while (i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

// Simply makes a duplicate string with size N not considering expansions.

char	*mini_strdup(char *s)
{
	char	*dup;
	size_t	i;

	i = 0;
	dup = arena_malloc((ft_strlen(s)) * sizeof(char));
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
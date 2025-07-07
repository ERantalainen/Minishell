/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:43:47 by erantala          #+#    #+#             */
/*   Updated: 2025/07/07 15:58:43 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	word_len(char *s, int quote)
{
	int		i;
	char	c;

	i = 0;
	while (s[i] && ft_isspace(s[i]) == 0 && !check_specials(s[i], quote))
		i++;
	if (s[i] && i == 0 && check_specials(s[i], quote))
	{
		if (s[i] == '|')
			return (1);
		c = s[i];
		if (!s[i + 1] || (s[i + 1] && s[i + 1] != c))
			return (1);
		else
			return (2);
	}
	return (i);
}

// Count the lenght of a word

int	check_specials(int c, int quote)
{
	if (quote == 0 && (c == '\'' || c == '"'))
		return (1);
	if (c == '<' || c == '|' || c == '>' || c == quote)
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
	t_data	*data;

	data = get_data();
	pos = 0;
	i = 0;
	len = expanded_length(s, n);
	dup = arena_malloc(len + 1);
	while (s[i] && i < n)
	{
		if (s[i] == '$' && s[i + 1])
			expans_help(s, dup, &i, &pos);
		else
			dup[pos++] = s[i++];
		if (s[i] && s[i] == '"')
			break ;
	}
	dup[pos] = '\0';
	return (dup);
}

char	*expans_help(char *s, char *dup, size_t *i, size_t *pos)
{
	char	*expansion;
	size_t	quotes;
	size_t	j;

	j = 0;
	quotes = 0;
	while(s[j])
	{
		if (s[j] == '"')
			quotes++;
		j++;
	}
	if (quotes % 2 == 0 || ft_isalnum(s[*i + 1]) || s[*i + 1] == '?')
		expansion = find_export(mini_strndup((s + (*i)), key_len(s + *i)));
	else
		expansion = mini_strdup("$");
	(*i) += key_len(s + *i);
	ft_strlcat(dup, expansion, ft_strlen(expansion) + 1 + (*pos));
	(*pos) += ft_strlen(expansion);
	return (expansion);
}

// Get the total length of dup once expanded. If an expansion ($) is detected
// Find the expansion, cat it onto the new string and adjust positions.
// Null terminate and return the result

size_t	expanded_length(char *s, size_t n)
{
	size_t	i;
	size_t	total;
	size_t	other;
	char	*exp;

	i = 0;
	total = 0;
	other = 0;
	while (s[i] && i < n)
	{
		if (s[i] == '$' && s[i + 1] != 0)
		{
			exp = find_export(mini_strndup(s + i, key_len(s + i)));
			if (ft_strcmp(exp, "") != 0)
				total += ft_strlen(exp);
			i += key_len(s + i);
		}
		else
		{
			other++;
			i++;
		}
	}
	return (other + total);
}

// Get total length of a token with expansions.

char	*mini_strndup(char *s, size_t n)
{
	char	*dup;
	size_t	i;

	i = 0;
	dup = arena_malloc(n + 1);
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
	dup = arena_malloc(ft_strlen(s) + 1);
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
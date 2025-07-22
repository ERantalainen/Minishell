/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_additions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:44:37 by erantala          #+#    #+#             */
/*   Updated: 2025/07/22 04:33:28 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*here_lim_token(char *s, size_t n, char quote)
{
	char	*dup;
	size_t	i;
	t_data	*data;

	data = get_data();
	i = 0;
	dup = arena_malloc(n + 1);
	while (quote != '\'' && quote != '"' && i < n)
		quote = s[i++];
	if (quote != '\'' && quote != '"')
		quote = 0;
	i = 0;
	while (i < n)
	{
		if (s[i] == quote)
			s += 1;
		if (!s[i] || i >= n)
			break ;
		dup[i] = s[i];
		i++;
	}
	if (quote != 0)
		data->last = HERE_NOEXP;
	dup[i] = '\0';
	return (dup);
}

// Makes the string for a heredoc delimiter.

char	*expand_quotes(char *s)
{
	size_t	i;
	char	*dupe;
	char	*exp;

	i = 0;
	dupe = "";
	while (s[i])
	{
		if (s[i] == '$' && s[i + 1])
		{
			exp = find_export(mini_strndup(s + i, key_len(s + i)));
			dupe = mini_join(dupe, exp);
			i += key_len(s + i);
		}
		else
		{
			dupe = mini_join(dupe, mini_strndup(&s[i], 1));
			i++;
		}
	}
	return (dupe);
}

static char	*here_token(char *s, size_t *i, int pos)
{
	char	*str;

	if (s[0] != '"' && s[0] != '\'')
	{
		str = here_lim_token(s, word_len(s, -1), 0);
		(*i) += word_len(s, -1);
	}
	else
	{
		str = mini_strndup(s + 1, pos - 1);
		(*i) += pos + 1;
	}
	return (str);
}

char	*quoted_token(char *s, char quote, size_t *i, t_type *last)
{
	char	*str;
	int		pos;

	pos = 1;
	while (s[pos] && s[pos] != quote && (quote == '"' || quote == '\''))
		pos++;
	if (*last == HERE_DOC || *last == HERE_NOEXP)
		str = here_token(s, i, pos);
	else
	{
		if (quote == '\'')
			str = mini_strndup(s + 1, pos - 1);
		else
			str = expand_quotes(mini_strndup(s + 1, pos - 1));
		if (s[pos] == quote)
			pos++;
		(*i) += pos;
		*last = QUOTED;
	}
	return (str);
}

size_t	quote_len(char *s, char quote)
{
	size_t	pos;

	pos = 0;
	if (s[pos] == '$')
		s += 1;
	if (s[pos] == '"')
		s += 1;
	while (s[pos] && s[pos] != quote)
		pos++;
	return (pos);
}

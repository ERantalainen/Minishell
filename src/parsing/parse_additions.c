/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_additions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:44:37 by erantala          #+#    #+#             */
/*   Updated: 2025/07/09 04:33:20 by erantala         ###   ########.fr       */
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
	while(quote != '\'' && quote != '"' && i < n)
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
	bool	space;
	
	i = 0;
	space = 0;
	dupe = "";
	while (s[i])
	{
		if (s[i] == '$')
		{
			exp = find_export(mini_strndup(s + i, key_len(s + i)));
			dupe = mini_join(dupe, exp);
			i += key_len(s + i);
		}
		else if (!ft_isspace(s[i]) || space == 0)
		{
			if (space == 0 && ft_isspace(s[i]))
				space = 1;
			else
				space = 0;
			dupe = mini_join(dupe, mini_strndup(&s[i], 1));
			i++;
		}
		else
			i++;
	}
	return (dupe);
}

char	*quoted_token(char *s, char quote, size_t *i, t_type *last)
{
	char	*str;
	int		pos;

	pos = 1;
	puts("here");
	if (*last == HERE_DOC && s[0] != '"' && s[0] != '\'')
	{
		str = here_lim_token(s, word_len(s, 0), quote);
		(*i) += word_len(s, 0);
	}
	else
	{
			printf("Pos :%s, %d:\n", s, pos);
		while (s[pos] && s[pos] != quote)
			pos++;
		if (quote == '\'')
			str = mini_strndup(s + 1, pos - 1);
		else
			str = expand_quotes(mini_strndup(s + 1, pos - 1));
		if (s[pos] == quote)
			pos++;
		printf("Pos :%s, %d:\n", s + pos, pos);
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

// Creates a token with quotes

void	cmd_help(t_vector *tokens, size_t *i, t_token *token, t_cmd *cmd)
{
	while ((*i) < tokens->count && (token->t == STRING || token->t == FILES))
	{
		cmd->str = token->s;
		built_in(cmd);
		(*i)++;
		break;
	}
}

// Helper function for command parsing.
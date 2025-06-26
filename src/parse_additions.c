/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_additions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:44:37 by erantala          #+#    #+#             */
/*   Updated: 2025/06/27 01:56:16 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd *check_redirect(t_cmd *cmd, t_token *token)
{
	if (token->t != STRING)
		puts("dw about it");
	if (cmd->type == INPUT)
	{
		if (access(token->s, R_OK) != 0)
			perror("minishell");
		else
		{
			cmd->next = FILES;
			token->t = FILES;
		}
	}
	if (cmd->type == OUTPUT)
		cmd->next = FILES;
	return (cmd);

}

char	*mini_append(char *s1, char *s2)
{
	char	*dup;

	dup = arena_malloc(ft_strlen(s1) + ft_strlen(s2) + 2);
	dup[0] = '\0';
	ft_strlcat(dup, s1, ft_strlen(s1) + 1);
	if (s1[0] != 0)
		dup[ft_strlen(s1)] = ' ';
	ft_strlcat(dup, s2, ft_strlen(s1) + ft_strlen(s2) + 2);
	return (dup);
}

char	*quoted_token(char *s, char quote, size_t *i)
{
	char	*str;
	int		pos;

	pos = 1;
	while (s[pos] && s[pos] != quote)
		pos++;
	if (quote == '\'')
		str = mini_strndup(s + 1, pos - 1);
	else
		str = expand_strndup(s + 1, pos - 1);
	if (s[pos] == quote)
		pos++;
	(*i) += pos;
	return (str);
}

size_t	quote_len(char *s, char quote)
{
	size_t	pos;

	pos = 0;
	while (s[pos] && ft_isspace(s[pos]) == 0 && s[pos] != quote)
		pos++;
	pos--;
	return (pos);
}

// Creates a token with quotes


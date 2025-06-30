/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_additions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:44:37 by erantala          #+#    #+#             */
/*   Updated: 2025/06/30 18:54:39 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd *check_redirect(t_cmd *cmd, t_token *token)
{
	if (token->t != STRING && token->t != FILES)
		puts("dw about it");
	if (cmd->type == INPUT)
	{
		if (access(token->s, R_OK) != 0)
		{
			perror("minishell");
		}
		else
		{
			cmd->next = FILES;
			token->t = FILES;
		}
	}
	if (cmd->type == OUTPUT)
	{
		token->t = FILES;
		cmd->next = FILES;
	}
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
	if (s[pos] == '$')
		s += 1;
	if (s[pos] == '"')
		s += 1;
	while (s[pos] && ft_isspace(s[pos]) == 0 && s[pos] != quote)
		pos++;
	return (pos);
}

// Creates a token with quotes

void	cmd_help(t_vector *tokens, size_t *i, t_token *token, t_cmd *cmd)
{
	while ((*i) < tokens->count && token->t == STRING)
	{
		cmd->str = mini_append(cmd->str, token->s);
		(*i)++;
		if (*i >= tokens->count)
			break ;
		token = tokens->data[(*i)];
		if (token && access(token->s, R_OK | W_OK) == 0)
			break ;
	}
}

// Helper function for command parsing.
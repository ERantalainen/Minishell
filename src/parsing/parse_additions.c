/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_additions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:44:37 by erantala          #+#    #+#             */
/*   Updated: 2025/07/03 03:20:55 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*check_redirect(t_cmd *cmd, t_token *token)
{
	if (cmd->type == INPUT)
	{
		cmd->next = FILES;
		token->t = FILES;
	}
	if (cmd->type == OUTPUT && cmd->type == STRING)
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

char	*quoted_token(char *s, char quote, size_t *i, t_type last)
{
	char	*str;
	int		pos;

	pos = 1;
	if (last == HERE_DOC)
	{
		str = here_lim_token(s, word_len(s, 0), quote);
		(*i) += word_len(s, 0);
	}
	else
	{
	while (s[pos] && s[pos] != quote)
		pos++;
	if (quote == '\'')
		str = mini_strndup(s + 1, pos - 1);
	else
		str = expand_strndup(s + 1, pos - 1);
	if (s[pos] == quote)
		pos++;
	(*i) += pos;
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
	while (s[pos] && ft_isspace(s[pos]) == 0 && s[pos] != quote)
		pos++;
	return (pos);
}

// Creates a token with quotes

void	cmd_help(t_vector *tokens, size_t *i, t_token *token, t_cmd *cmd)
{
	while ((*i) < tokens->count && (token->t == STRING || token->t == FILES))
	{
		if (token->space == 1)
			cmd->str = mini_append(cmd->str, token->s);
		else
			cmd->str = mini_join(cmd->str, token->s);
		(*i)++;
		if (*i >= tokens->count)
			break ;
		token = tokens->data[(*i)];
		if (token && access(token->s, R_OK) == 0)
			break ;
	}
}

// Helper function for command parsing.

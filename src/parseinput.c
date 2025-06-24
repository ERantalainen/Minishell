/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseinput.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:38:10 by erantala          #+#    #+#             */
/*   Updated: 2025/06/24 15:13:12 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// MAKE EXCEPTONS FOR <<<A <>A ><A
// https://www.gnu.org/software/bash/manual/bash.html#Shell-Syntax

t_token	*create_token(char *s, size_t *i)
{
	t_token	*new;

	new = arena_malloc(sizeof(t_token));
	new->s = token_string(s, i);
	if (ft_strncmp(new->s, "|", 1) == 0)
		new->t = PIPE;
	else if (ft_strcmp(new->s, "<") == 0)
		new->t = INPUT;
	else if (ft_strcmp(new->s, ">") == 0)
		new->t = OUTPUT;
	else if (ft_strncmp(new->s, "<<", 2) == 0)
		new->t = HERE_DOC;
	else if (ft_strncmp(new->s, ">>", 2) == 0)
		new->t = APPEND;
	else
		new->t = STRING;
	return (new);
}

// check for token type

t_vector	*token_vector(char *s)
{
	size_t	i;
	t_vector	*tokens;
	t_token		*token;

	tokens = new_vector(4);
	i = 0;
	while (s[i])
	{
		while (s[i] && ft_isspace(s[i]) == 1)
			i++;
		token = create_token(s, &i);
		if (token->s)
			add_elem(tokens, token);
		while (s[i] && ft_isspace(s[i]) == 1)
			i++;
	}
	check_heredoc(tokens);
	return (tokens);
}


char	*token_string(char	*s, size_t	*i)
{
	char	*token;
	int		len;

	if (s[(*i)] == '\'' || s[(*i)] == '"')
		return (quoted_token(s + *i, s[(*i)], i));
	len = word_len(s + (*i));
	token = expand_strndup(s + (*i), len);
	(*i) += len;
	return (token);
}

t_vector	*create_commands(t_vector *tokens)
{
	t_vector	*commands;
	t_token		*curr;
	size_t		i;

	i = 0;
	commands = new_vector(tokens->count + 1);
	while (i < tokens->count && tokens->data[i] != NULL)
	{
		curr = tokens->data[i];
		if (curr->t == STRING)
			add_elem(commands, make_cmd_str(tokens, &i));
		else
			add_elem(commands, make_cmd_spc(tokens, &i));
	}
	return (commands);
}

t_cmd	*make_cmd_str(t_vector *tokens, size_t *i)
{
	t_token	*token;
	t_cmd	*cmd;

	cmd = arena_malloc(sizeof(t_cmd));
	token = tokens->data[(*i)];
	cmd->type = STRING;
	cmd->str = "";
	if (access(token->s, R_OK | W_OK) != 0)
	{
		while ((*i) < tokens->count && token->t == STRING)
		{
			cmd->str = mini_append(cmd->str, token->s);
			(*i)++;
			token = tokens->data[(*i)];
			if (token && access(token->s, R_OK | W_OK) == 0)
				break ;
		}
	}
	else
	{
		cmd->type = FILES;
		cmd->str = token->s;
	}
	if ((*i) < tokens->count)
		cmd->next = token->t;
	else
		cmd->next = EMPTY;
	return (cmd);
}

t_cmd	*make_cmd_spc(t_vector *tokens, size_t *i)
{
	t_token	*token;
	t_cmd	*cmd;

	cmd = arena_malloc(sizeof(t_cmd));
	token = tokens->data[(*i)];
	cmd->type = token->t;
	cmd->str = token->s;
	(*i)++;
	if ((*i) < tokens->count)
	{
		token = tokens->data[(*i)];
		cmd->next = token->t;
	}
	if (cmd->type == INPUT || cmd->type == OUTPUT)
	else
		cmd->next = EMPTY;
	return (cmd);
}

t_cmd *check_redirect(t_cmd *cmd, t_token *token)
{
	if (token->t != STRING)
		ft_exit("Invalid file name", 1);
	if (cmd->type == INPUT)
	{
		if (access(token->s, R_OK) != 0)
			exit(1);
		else
		{
			token->t = FILES;
			cmd->next = FILES;
		}
	}
	if (cmd->type == OUTPUT)
	{
		cmd->next = FILES;
		token->t = FILES;
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
	str = mini_strndup(s + 1, pos);
	if (s[pos] == quote)
		pos++;
	(*i) += pos;
	return (str);
}

// Creates a token with quotes

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

int	check_specials(int	c)
{
	if (c == '<' || c == '|' || c == '>')
		return (1);
	return (0);
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
	dup = arena_malloc(len + 1);
	while (s[i] && i < n && !ft_isspace(s[i]))
	{
		if (s[i] == '$')
		{
			expansion = find_export(s + i);
			i += word_len(s + i);
			ft_strlcat(dup, expansion, ft_strlen(expansion) + 1 + pos);
			pos += ft_strlen(expansion);
			if (i >= n)
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
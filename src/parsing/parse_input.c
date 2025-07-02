/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:38:10 by erantala          #+#    #+#             */
/*   Updated: 2025/07/02 16:08:53 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// MAKE EXCEPTONS FOR <<<A <>A ><A
// https://www.gnu.org/software/bash/manual/bash.html#Shell-Syntax

t_token	*create_token(char *s, size_t *i, t_type last)
{
	t_token	*new;

	new = arena_malloc(sizeof(t_token));
	new->s = token_string(s, i);
	new->space = 1;
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
	else if (access(new->s, F_OK) == 0)
		new->t = FILES;
	else
		new->t = STRING;
	if ((last == HERE_DOC && new->t == STRING )
		&& (s[(*i)] == '"' || s[(*i)] == '\''))
			new->t = HERE_NOEXP;
	return (new);
}

// check for token type

t_vector	*token_vector(char *s)
{
	size_t		i;
	t_vector	*tokens;
	t_token		*token;
	size_t		len;

	len = ft_strlen(s);
	tokens = new_vector(4);
	i = 0;
	while (s[i])
	{
		while (s[i] && ft_isspace(s[i]) == 1)
			i++;
		if (tokens->count == 0)
			token = create_token(s, &i, EMPTY);
		else
			token = create_token(s, &i, token->t);
		printf("Token %zu: %s T: %d\n", tokens->count, token->s, token->t);
		if (token->s)
			add_elem(tokens, token);
		if (s[i] && !ft_isspace(s[i]))
			token->space = 0;
		if (i >= len)
			break ;
	}
	if (tokens->count == 0)
		return (NULL);
	check_heredoc(tokens);
	check_repeat(tokens);
	return (tokens);
}

// Create token vector

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
	commands = new_vector(tokens->count + 1);
	while (i < tokens->count && tokens->data[i] != NULL)
	{
		curr = tokens->data[i];
		if (curr->t == STRING || curr->t == FILES)
			add_elem(commands, make_cmd_str(tokens, &i));
		else
			add_elem(commands, make_cmd_spc(tokens, &i));
	}
	next_check(commands);
	return (commands);
}

// Combine tokens into commands.

t_cmd	*make_cmd_str(t_vector *tokens, size_t *i)
{
	t_token	*token;
	t_cmd	*cmd;

	cmd = arena_malloc(sizeof(t_cmd));
	token = tokens->data[(*i)];
	cmd->type = STRING;
	cmd->str = "";
	if (access(token->s, R_OK | W_OK) != 0 || (*i == 0))
		cmd_help(tokens, i, token, cmd);
	else
	{
		cmd->type = FILES;
		cmd->str = token->s;
		(*i)++;
	}
	if ((*i) < tokens->count)
		cmd->next = token->t;
	else
		cmd->next = EMPTY;
	built_in(cmd);
	return (cmd);
}

// Make a command of type FILES or STRING

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
		if (cmd->type == INPUT || cmd->type == OUTPUT)
			check_redirect(cmd, token);
		else
			cmd->next = token->t;
	}
	else
		cmd->next = EMPTY;
	return (cmd);
}

// Make a redirect or pipe command



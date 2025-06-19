/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseinput.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:38:10 by erantala          #+#    #+#             */
/*   Updated: 2025/06/19 03:36:32 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *s)
{
	t_token	*new;

	new = arena_malloc(sizeof(t_token));
	new->s = token_string(s);
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
		while(ft_isspace(s[i]) && s[i])
			i++;
		if (!s[i])
			break ;
		token = create_token(s + i);
		add_elem(tokens, token);
		while (ft_isspace(s[i]) == 0 && s[i])
			i++;
	}
	return (tokens);
}
char	*token_string(char	*s)
{
	char	*token;
	int		pos;
	int		len;

	pos = 0;
	while (s[pos] && (ft_isspace(s[pos]) == 0))
		pos++;
	len = pos;
	pos = 0;
	token = arena_malloc(len + 1);
	while (pos < len)
	{
		token[pos] = s[pos];
		pos++;
	}
	token[pos] = '\0';
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
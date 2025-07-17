/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:01 by erantala          #+#    #+#             */
/*   Updated: 2025/07/18 02:06:41 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	skip_redirects_echo(t_cmd **tokens, size_t tok_i)
{
	while (tokens[tok_i] && (tokens[tok_i]->type == OUTPUT
			|| tokens[tok_i]->type == APPEND
			|| tokens[tok_i]->type == HERE_DOC
			|| tokens[tok_i]->type == INPUT))
	{
		tok_i++;
		if (tokens[tok_i] && (tokens[tok_i]->type == FILES
				|| tokens[tok_i]->type == STRING))
		{
			tok_i++;
			continue ;
		}
	}
	return (tok_i);
}

static char	*build_echo_string(t_cmd **c, int *i, char *part)
{
	while (c[*i] && (c[*i]->type == FILES || c[*i]->type == STRING))
	{
		if (c[*i]->space)
			part = mini_append(part, c[*i]->str);
		else
			part = mini_join(part, c[*i]->str);
		(*i)++;
	}
	*i = skip_redirects_echo(c, *i);
	if (c[*i] && (c[*i]->type == FILES || c[*i]->type == STRING))
		part = mini_append(part, "");
	return (part);
}

static char	*process_echo_arguments(t_cmd **c, bool *nl, int *i, char *part)
{
	int	pos;

	pos = *i;
	part = "";
	while (c[*i] && (c[*i]->type == FILES || c[*i]->type == STRING))
	{
		c[*i]->space = 0;
		if (echo_part(c, &pos, nl, 0) == -1)
		{
			part = build_echo_string(c, i, part);
			if (c[*i] && (c[*i]->type == FILES || c[*i]->type == STRING))
				continue ;
			break ;
		}
		*i = pos;
		*i = skip_redirects_echo(c, *i);
	}
	return (part);
}

void	echo(t_cmd **cmd, int i)
{
	char	*command;
	bool	newline;

	if (!cmd[i])
	{
		if (write(1, "\n", 1) < 0)
			perror("write");
		return ;
	}
	command = "";
	newline = 1;
	command = process_echo_arguments(cmd, &newline, &i, command);
	if (newline == 1)
		ft_putendl_fd((mini_strndup(command, ft_strlen(command))), 1);
	else
		ft_putstr_fd((mini_strndup(command, ft_strlen(command))), 1);
	replace_export("?=0");
}

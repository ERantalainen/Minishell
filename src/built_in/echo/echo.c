/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:01 by erantala          #+#    #+#             */
/*   Updated: 2025/07/15 05:24:57 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_echo_arguments(t_cmd **c,
					bool *newline, int *i, char *part)
{
	int		pos;
	bool	quoted;

	pos = *i;
	while (c[*i] && (c[*i]->type == FILES || c[*i]->type == STRING))
	{
		c[*i]->space = 0;
		quoted = 0;
		if (echo_part(c, &pos, newline, quoted) == -1)
		{
			part = "";
			while (c[*i] && (c[*i]->type == FILES || c[*i]->type == STRING))
			{
				if (c[*i]->space)
					part = mini_append(part, c[*i]->str);
				else
					part = mini_join(part, c[*i]->str);
				(*i)++;
			}
			break ;
		}
		(*i) = pos;
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:01 by erantala          #+#    #+#             */
/*   Updated: 2025/07/10 14:22:31 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	valid_option(char *s)
{
	int	i;

	i = 0;
	if (s[i] != '-')
		return(-1);
	i++;
	while (s[i] && s[i] == 'n')
	{
		i++;
	}
	if (s[i] && s[i] != 'n')
		return (-1);
	else
		return (i);
}

static	void options(char *command, int *i, bool *nl)
{
	char	*check_option;

	if ((command[*i] == '-' && command[*i + 1] == 'n'))
	{
		while (1 && command[*i])
		{
			check_option = mini_strndup(command + *i, word_len(command + *i, 0));
			if (valid_option(check_option) == -1)
				break ;
			else
				*i += word_len(command + *i, 0);
			*nl = 0;
			while(ft_isspace(command[*i]))
				(*i)++;
		}
	}
}

void	echo(t_cmd **cmd, int i)
{
	char	*command;
	int		pos;
	bool	newline;

	if (!cmd[i])
	{
		write(1, "\n", 1);
		return ;
	}
	command = mini_strdup(cmd[i]->str);
	if (cmd[i] && (cmd[i]->next == FILES || cmd[i]->next == STRING || cmd[i]->next == BUILTIN))
		i++;
	while (cmd[i] && (cmd[i]->next == FILES || cmd[i]->next == STRING || cmd[i]->next == BUILTIN))
	{
		if (cmd[i]->space)
			command = mini_append(command, cmd[i]->str);
		else
			command = mini_join(command, cmd[i]->str);
		i++;
	}
	if (cmd[i] && (cmd[i]->next == FILES || cmd[i]->next == STRING || cmd[i]->next == BUILTIN))
	{
		if (cmd[i] && !cmd[i]->next && cmd[i]->space)
			command = mini_append(command, cmd[i]->str);
		else if (cmd[i])
			command = mini_join(command, cmd[i]->str);
	}
	pos = 0;
	newline = 1;
	options(command, &pos, &newline);
	command += pos;

	if (newline == 1)
		ft_putendl_fd((mini_strndup(command, ft_strlen(command))), 1);
	else
		ft_putstr_fd((mini_strndup(command, ft_strlen(command))), 1);
	replace_export("?=0");
}

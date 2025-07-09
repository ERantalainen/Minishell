/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:01 by erantala          #+#    #+#             */
/*   Updated: 2025/07/09 15:23:59 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



static	void options(char *command, int *i, bool *nl)
{
	t_data	*data;

	data = get_data();
	if ((command[*i] == '-' && command[*i + 1] == 'n' &&
		(!command[*i + 2] || command[*i + 2]  == ' ' || command[*i + 2] == 'n')))
	{
		while (command[*i] && ((command[*i] == '-' && command[*i + 1] == 'n')
		|| (command[*i] == 'n' && (command[*i + 1] == 'n' || command[*i + 1] == ' ' || !command[*i + 1]))
		|| (command[*i] == ' ' || command[*i + 1] == ' ' || command[*i + 1] == '-')))
			(*i)++;
		*nl = 0;
	}
}

void	echo(t_cmd **cmd, int i)
{
	char	*command;
	bool	newline;

	ft_fprintf(2, "Echo: %d: %s %d %d\n", i, cmd[i]->str, cmd[i]->space, cmd[i]->quoted);
	command = mini_strdup(cmd[i]->str);
	i++;
	while (cmd[i] && (cmd[i]->next == FILES || cmd[i]->next == STRING))
	{
		ft_fprintf(2, "Loop1: %d: [%s] %d %d\n", i, cmd[i]->str, cmd[i]->space, cmd[i]->quoted);
		puts(command);
		if (cmd[i]->space && cmd[i - 1]->space)
		{
			command = mini_append(command, cmd[i]->str);
			puts("here");
		}
		else
			command = mini_join(command, cmd[i]->str);
		puts(command);
		ft_fprintf(2, "Loop2: %d: [%s] %d %d\n", i, cmd[i]->str, cmd[i]->space, cmd[i]->quoted);
		i++;
	}
	if (!cmd[i]->next && cmd[i]->space && cmd[i - 1]->space)
		command = mini_append(command, cmd[i]->str);
	ft_fprintf(2, "%d: %s %d %d\n", i, cmd[i]->str, cmd[i]->space, cmd[i]->quoted);
	puts(command);
	i = 0;
	newline = 1;
	options(command, &i, &newline);
	command += i;
	if (newline == 1)
		ft_putendl_fd((mini_strndup(command, ft_strlen(command))), 1);
	else
		ft_putstr_fd((mini_strndup(command, ft_strlen(command))), 1);
	replace_export("?=0");
}

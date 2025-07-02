/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:01 by erantala          #+#    #+#             */
/*   Updated: 2025/07/02 19:48:45 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(t_cmd **commands, int i)
{
	char	*command;
	bool	newline;

	command = commands[i]->str + 4;
	while (commands[i]->next == FILES || commands[i]->next == STRING)
		command = mini_append(command, commands[i++ + 1]->str);
	i = 0;
	while(command[i] == ' ')
		i++;
	newline = 1;
	if ((command[i] == '-' && command[i + 1] == 'n'))
	{
		while (command[i] == 'n' || command[i] == ' ' || (command[i] == '-'
			&& command[i + 1] != ' ' && command[i + 1] != '-'))
			i++;
		newline = 0;
		if (command[i] == ' ')
			i++;
	}
	command += i;
	if (newline == 1)
		ft_putendl_fd((mini_strndup(command, ft_strlen(command))), 1);
	else
		ft_putstr_fd((mini_strndup(command, ft_strlen(command))), 1);
	replace_export("?=0");
}

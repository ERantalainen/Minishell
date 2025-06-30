/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:01 by erantala          #+#    #+#             */
/*   Updated: 2025/06/30 19:36:25 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(t_cmd **commands, int i)
{
	char	*command;
	bool	newline;
	size_t	pos;

	pos = 0;
	command = commands[i]->str + 5;
	while (commands[i]->next == FILES || commands[i]->next == STRING)
		command = mini_join(command, commands[i++ + 1]->str);
	newline = 1;
	if (command[pos] == '-' && command[pos + 1] == 'n')
	{
		pos++;
		while (command[pos] == 'n')
			pos++;
		newline = 0;
		if (command[pos] == ' ')
			command += pos + 1;
	}
	if (newline == 1)
		ft_putendl_fd((mini_strndup(command, ft_strlen(command))), 1);
	else
		ft_putstr_fd((mini_strndup(command, ft_strlen(command))), 1);
	replace_export("?=0");
}

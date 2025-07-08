/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:01 by erantala          #+#    #+#             */
/*   Updated: 2025/07/08 19:49:01 by erantala         ###   ########.fr       */
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

void	echo(t_cmd **commands, int i)
{
	char	*command;
	bool	newline;

	command = commands[i]->str + 5;
	while (commands[i]->next == FILES || commands[i]->next == STRING)
		command = mini_append(command, commands[i++ + 1]->str);
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

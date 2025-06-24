/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:01 by erantala          #+#    #+#             */
/*   Updated: 2025/06/24 17:58:26 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(t_vector *commands, int i)
{
	t_cmd	*cmd;
	char	*command;
	char	*output;
	bool	newline;
	size_t	pos;

	pos = 0;
	cmd = commands->data[i];
	command = cmd->str + 5;
	newline = true;
	if (command[pos] == '-' && command[pos + 1] == 'n')
	{
		newline = false;
		command += 2;
	}
	output = mini_strndup(command, ft_strlen(command));
	if (newline == true)
		ft_putendl_fd(command, 1);
	else
		ft_putstr_fd(command, 1);
}

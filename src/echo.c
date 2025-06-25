/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:01 by erantala          #+#    #+#             */
/*   Updated: 2025/06/25 18:15:09 by erantala         ###   ########.fr       */
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
	newline = 1;
	if (command[pos] == '-' && command[pos + 1] == 'n')
	{
		newline = 0;
		command += 3;
	}
	output = mini_strndup(command, ft_strlen(command));
	if (newline == 1)
		ft_putendl_fd(output, 1);
	else
		ft_putstr_fd(output, 1);
}

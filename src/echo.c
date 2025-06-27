/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:01 by erantala          #+#    #+#             */
/*   Updated: 2025/06/27 15:32:06 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(t_cmd *commands)
{
	char	*command;
	char	*output;
	bool	newline;
	size_t	pos;

	pos = 0;
	command = commands->str + 5;
	newline = 1;
	if (command[pos] == '-' && command[pos + 1] == 'n')
	{
		while (command[pos + 1] == 'n')
			pos++;
		newline = 0;
		command += pos + 2;
	}
	output = mini_strndup(command, ft_strlen(command));
	if (newline == 1)
		ft_putendl_fd(output, 1);
	else
		ft_putstr_fd(output, 1);
}

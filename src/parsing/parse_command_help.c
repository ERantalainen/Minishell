/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_help.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 02:20:24 by erantala          #+#    #+#             */
/*   Updated: 2025/07/11 04:09:22 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_trim_check(t_vector *commands)
{
	t_cmd	*cmd;
	size_t	i;

	i = 0;
	cmd = (t_cmd *)commands->data[1];
	if (!cmd || (cmd->type != STRING && cmd->type != FILES))
		return ;
	if (!cmd->quoted)
	{
		while (cmd->str[i] && ft_isspace(cmd->str[i]))
			i++;
	}
	else
		return ;
	cmd->str += i;
	commands->data[1] = cmd;
}
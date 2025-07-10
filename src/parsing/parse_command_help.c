/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_help.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 02:20:24 by erantala          #+#    #+#             */
/*   Updated: 2025/07/11 02:27:27 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		trim_check(t_vector *commands)
{
	t_cmd	**cmd;
	size_t	i;

	i = 0;
	cmd = (t_cmd **)commands->data;
	if (!cmd[1] || (cmd[1]->type != STRING && cmd[1]->type != FILES))
		return ;
	if (!cmd[1]->quoted)
	{
		while (cmd[1]->str[i] && ft_isspace(cmd[1]->str[i]))
			i++;
	}
	else
		return ;
	cmd[1]->str += i;
}
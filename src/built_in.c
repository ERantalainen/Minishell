/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:56:30 by erantala          #+#    #+#             */
/*   Updated: 2025/06/24 15:52:37 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*built_in(t_vector *cmds)
{
	char	*s;
	t_cmd	*cmd;

	cmd = (t_cmd *)cmds->data[0];
	if (ft_strnmcp("echo", cmd->str, 4) == 0)
		ft_echo(cmds);
	if (ft_strnmcp("export", cmd->str, 6) == 0);
		export()
}
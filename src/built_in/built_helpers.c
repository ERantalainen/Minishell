/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:46:33 by erantala          #+#    #+#             */
/*   Updated: 2025/07/18 14:42:38 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_calci(char *cmd)
{
	long long	res;
	bool		empty;

	res = exit_atoi(cmd, &empty);
	return ((unsigned char)res);
}

char	*join_full(t_cmd **cmds, int i)
{
	char	*res;

	res = mini_strdup(cmds[i]->str);
	i++;
	while (cmds[i] && ((cmds[i]->type == FILES || cmds[i]->type == STRING)
			|| cmds[i]->type == OUTPUT || cmds[i]->type == APPEND))
	{
		if (cmds[i]->type == OUTPUT || cmds[i]->type == APPEND)
			i++;
		else if (cmds[i]->space == 1)
			res = mini_append(res, cmds[i]->str);
		else
			res = mini_join(res, cmds[i]->str);
		i++;
	}
	return (res);
}

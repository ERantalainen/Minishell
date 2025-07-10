/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:46:33 by erantala          #+#    #+#             */
/*   Updated: 2025/07/10 01:52:50 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_full(t_cmd **cmds, int i)
{
	char	*res;

	res = mini_strdup(cmds[i]->str);
	i++;
	while (cmds[i] && (cmds[i]->type == FILES || cmds[i]->type == STRING))
	{
		if (cmds[i]->space == 1)
			res = mini_append(res, cmds[i]->str);
		else
			res = mini_join(res, cmds[i]->str);
		i++;
	}

	return (res);
}

char	*exit_join(t_cmd **cmds, int i)
{
	char	*res;

	res = mini_strdup(cmds[i]->str);
	i++;
	while (cmds[i] && (cmds[i]->type == FILES || cmds[i]->type == STRING))
	{
		if (cmds[i]->space == 1)
			res = mini_append(res, cmds[i]->str);
		else
			res = mini_join(res, cmds[i]->str);
		i++;
	}
	return (res);
}
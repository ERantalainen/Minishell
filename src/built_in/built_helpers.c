/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:46:33 by erantala          #+#    #+#             */
/*   Updated: 2025/07/08 21:10:55 by jpelline         ###   ########.fr       */
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
		res = mini_append(res, cmds[i]->str);
		i++;
	}
	return (res);
}
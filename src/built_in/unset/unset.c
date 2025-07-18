/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:37:46 by erantala          #+#    #+#             */
/*   Updated: 2025/07/11 03:22:44 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_unset(t_cmd **cmds, int i)
{
	char	*cmd;

	cmd = "";
	i = skip_redirects(cmds, i);
	while (cmds[i] && ((cmds[i]->type == FILES || cmds[i]->type == STRING)))
	{
		if (cmds[i]->space == 0)
		{
			cmd = mini_join(cmd, cmds[i]->str);
			i++;
		}
		else
			cmd = mini_strdup(cmds[i]->str);
		if (cmds[i] && cmds[i]->space != 0)
		{
			unset(cmd);
			cmd = arena_malloc(1);
		}
		i++;
		i = skip_redirects(cmds, i);
	}
}

void	unset(char *key)
{
	t_data	*data;
	size_t	i;
	char	*var;

	data = get_data();
	i = 0;
	while (i < data->env_vec->count)
	{
		var = data->env_vec->data[i];
		if (ft_strncmp(key, var, key_len(var)) == 0)
		{
			remove_elem(data->env_vec, i);
			init_export();
			replace_export("?=0");
			return ;
		}
		i++;
	}
	return ;
}

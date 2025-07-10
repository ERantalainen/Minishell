/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:37:46 by erantala          #+#    #+#             */
/*   Updated: 2025/07/10 22:59:53 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_unset(t_cmd **cmds, int i)
{
	char	*cmd;

	cmd = "";
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
			cmd = "";
		}
		i++;
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
			return ;
		}
		i++;
	}
	return ;
}

char	**unset_child(char	*key, char **envi)
{
	int	i;

	i = find_line(envi, key);
	if (i == -1)
		exit(1);
	else
		ft_strremove(envi, i);
	exit (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:37:46 by erantala          #+#    #+#             */
/*   Updated: 2025/07/04 15:32:26 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(char *key)
{
	t_data	*data;
	size_t	i;
	char	*var;

	data = get_data();
	i = 0;
	if (key[0] == '$')
		key += 1;
	while (i < data->env_vec->count)
	{
		var = data->env_vec->data[i];
		if (ft_strncmp(key, var, key_len(var)) == 0)
		{
			remove_elem(data->env_vec, i);
			break ;
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

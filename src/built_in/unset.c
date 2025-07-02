/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:37:46 by erantala          #+#    #+#             */
/*   Updated: 2025/06/26 15:26:51 by erantala         ###   ########.fr       */
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

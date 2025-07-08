/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:37:46 by erantala          #+#    #+#             */
/*   Updated: 2025/07/08 17:17:10 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_unset(char *command)
{
	int		len;
	size_t	i;

	while (command[0])
	{
		i = 0;
		while (command[i] && ft_isspace(command[i]))
			i++;
		if (!command[i])
			return ;
		len = word_len(command + i, 0);
		if (len > 0)
			unset(mini_strndup(command + i, len));
		else
			return ;
		command += len + i;
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

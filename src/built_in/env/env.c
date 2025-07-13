/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:19:17 by erantala          #+#    #+#             */
/*   Updated: 2025/07/13 20:30:14 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(void)
{
	t_data	*data;
	char	*line;
	size_t	i;

	i = 0;
	data = get_data();
	while (i < data->env_vec->count)
	{
		line = data->env_vec->data[i];
		if (ft_strncmp(line, "?=", 2) != 0 && ft_strchr(line, '='))
			printf("%s\n", line);
		i++;
	}
	replace_export("?=0");
}

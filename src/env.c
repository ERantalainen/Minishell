/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:19:17 by erantala          #+#    #+#             */
/*   Updated: 2025/06/26 00:48:26 by erantala         ###   ########.fr       */
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
	while(i < data->env_vec->count)
	{
		line = data->env_vec->data[i];
		printf("%s\n", line);
		i++;
	}
}
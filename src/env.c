/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:19:17 by erantala          #+#    #+#             */
/*   Updated: 2025/06/25 21:23:05 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(void)
{
	t_data	*data;
	size_t	i;

	i = 0;
	data = get_data();
	while(data->environ[i])
	{
		printf("%s\n", data->environ[i]);
		i++;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:02:28 by erantala          #+#    #+#             */
/*   Updated: 2025/06/24 15:16:20 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*get_data()
{
	static t_data data;

	return (&data);
}

void	ft_exit(char *s, int code)
{
	t_data	*data;
	int		i;
	int		*fd;
	i = 0;
	data = get_data();
	free_arenas();
	while (i < data->fds->count)
	{
		fd = (int *)data->fds->data[i];
	}

	ft_printf(2, "%s\n", s);
	exit(code);
}
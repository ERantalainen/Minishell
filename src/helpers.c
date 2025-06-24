/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:02:28 by erantala          #+#    #+#             */
/*   Updated: 2025/06/24 18:56:58 by erantala         ###   ########.fr       */
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
	size_t		i;
	int		*fd;
	i = 0;
	data = get_data();
	free_arenas();
	rl_clear_history();
	while (i < data->fds->count)
	{
		fd = (int *)data->fds->data[i];
		close(*fd);
		i++;
	}
	ft_fprintf(2, "%s\n", s);
	exit(code);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:07:45 by erantala          #+#    #+#             */
/*   Updated: 2025/06/27 17:13:15 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_heredoc()
{
	size_t	i;
	t_data	*data;

	data = get_data();
	i = 0;
	while (i < data->heredocs->count)
	{
		if (unlink((char *)data->heredocs->data[i]) == -1)
			perror("minisell: ");
		i++;
	}
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
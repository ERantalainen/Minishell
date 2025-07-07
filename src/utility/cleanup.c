/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:07:45 by erantala          #+#    #+#             */
/*   Updated: 2025/07/02 22:14:03 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_heredoc(void)
{
	size_t	i;
	t_data	*data;

	data = get_data();
	i = 0;
	while (i < data->heredocs->count)
	{
		close(data->hdfd[i]);
		if (unlink((char *)data->heredocs->data[i]) == -1)
			perror("minishell: ");
		i++;
	}
	data->heredocs = new_vector(2);
}

void	ft_exit(char *s, int code)
{
	t_data	*data;
	size_t	i;
	int		*fd;

	i = 0;
	data = get_data();
	rl_clear_history();
	while (i < data->fds->count)
	{
		fd = (int *)data->fds->data[i];
		close(*fd);
		i++;
	}
	if (s[0] != 0)
		ft_fprintf(2, "%s\n", s);
	free_arenas();
	exit(code);
}

void	ft_exit_child(char *s, int code)
{
	int i = 3;
	while (i < 1024)
	{
		close(i);
		i++;
	}
	if (s)
		ft_fprintf(2, "%s\n", s);
	exit(code);
}

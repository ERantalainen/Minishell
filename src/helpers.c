/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:02:28 by erantala          #+#    #+#             */
/*   Updated: 2025/06/25 00:31:28 by erantala         ###   ########.fr       */
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

void	init_data(void)
{
	t_data	*data;
	char	*path;
	int		len;

	data = get_data();
	len = 100;
	while (1)
	{
		path = arena_malloc(len);
		path = getcwd(path, 100);
		if (!path && errno == ERANGE)
			len += 25;
		else if (!path)
			ft_exit("Can't get path", errno);
		else
			break ;
	}
	data->directory = path;
}

char	*get_pwd()
{
	char	*path;
	int		len;

	len = 100;
	while (1)
	{
		path = arena_malloc(len);
		path = getcwd(path, 100);
		if (!path && errno == ERANGE)
			len += 25;
		else if (!path)
			return (NULL);
		else
			break ;
	}
	return (path);
}

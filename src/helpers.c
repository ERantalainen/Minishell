/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:02:28 by erantala          #+#    #+#             */
/*   Updated: 2025/06/26 00:46:11 by erantala         ###   ########.fr       */
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

size_t	ft_stralen(char **s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	init_data(char	**env)
{
	t_data	*data;

	data = get_data();
	data->directory = get_pwd();
	data->environ = env;
	data->env_vec = new_vector(ft_stralen(env));
	array_to_vec(data->env_vec, (void **)env);

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

char	*absolute_path(char *relative)
{
	char	*path;
	char	*absolut;

	path = get_pwd();
	if (!path)
		exit(1);
	absolut = arena_malloc(ft_strlen(path) + ft_strlen(relative) + 1);
	ft_strlcat(absolut, path, ft_strlen(path) + 1);
	ft_strlcat(absolut, relative, ft_strlen(absolut + ft_strlen(relative))+ 1);
	return (absolut);
}
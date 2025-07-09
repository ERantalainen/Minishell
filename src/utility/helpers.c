/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:02:28 by erantala          #+#    #+#             */
/*   Updated: 2025/07/09 19:35:19 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*get_data(void)
{
	static t_data	data;

	return (&data);
}

size_t	ft_stralen(char **s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	init_data(char **env)
{
	t_data	*data;

	data = get_data();
	data->directory = get_pwd();
	data->fds = new_vector(5);
	data->environ = env;
	data->line = 1;
	data->env_vec = new_vector(ft_stralen(env));
	data->heredocs = new_vector(2);
	data->last = EMPTY;
	data->check_build = 1;
	array_to_vec(data->env_vec, (void **)env);
	add_elem(data->env_vec, mini_strdup("?=0"));
	init_export();
	data->trash = mini_join(find_export("HOME"), "/.local/share/Trash/files");
}

char	*get_pwd(void)
{
	char	*path;
	int		len;

	len = 100;
	while (1)
	{
		path = arena_malloc(len);
		path = getcwd(path, len);
		if (!path && errno == ERANGE)
			len += 25;
		else if (!path)
		{
			perror("minishell: pwd: ");
			return (NULL);
		}
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
	ft_strlcat(absolut, relative, ft_strlen(absolut + ft_strlen(relative)) + 1);
	return (absolut);
}


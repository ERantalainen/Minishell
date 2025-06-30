/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:02:28 by erantala          #+#    #+#             */
/*   Updated: 2025/06/30 19:34:06 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*get_data()
{
	static t_data data;

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

void	init_data(char	**env)
{
	t_data	*data;

	data = get_data();
	data->directory = get_pwd();
	data->fds = new_vector(5);
	data->environ = env;
	data->line = 1;
	data->env_vec = new_vector(ft_stralen(env));
	data->heredocs = new_vector(2);
	array_to_vec(data->env_vec, (void **)env);
	make_export("?=0");
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

void	child_died(int status)
{
	const char	*exit_code = "?=";
	char		*exit_export;

	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_fprintf(STDERR_FILENO, "%s", QUIT);
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_done = 1;
		exit_export = mini_join(exit_code, mini_itoa(WTERMSIG(status) + 128));
		if (ft_strcmp(find_export("?"), "") == 0)
			export(exit_export);
		else
			replace_export(exit_export);
	}
	else if (ft_strcmp(find_export("?"), "") == 0)
			export(mini_join(exit_code, mini_itoa(WEXITSTATUS(status))));
	else
		replace_export(mini_join(exit_code, mini_itoa(WEXITSTATUS(status))));
}

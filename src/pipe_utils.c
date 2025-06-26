/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:35:04 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/26 18:55:08 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_cmd_args(char *cmd, char *path)
{
	char	**args;
	char	**cmd_args;
	int		i;

	args = ft_split(cmd, ' ');
	i = 0;
	while (args[i])
		i++;
	cmd_args = arena_malloc((i + 1) * sizeof(char *));
	cmd_args[0] = ft_strdup(path);
	i = 1;
	while (args[i])
	{
		cmd_args[i] = ft_strdup(args[i]);
		i++;
	}
	cmd_args[i] = NULL;
	return (cmd_args);
}

static char	**parse_paths(char **env)
{
	char	**env_paths;
	int		i;
	int		check;

	check = 0;
	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
		{
			check = 1;
			break ;
		}
		i++;
	}
	if (check == 0 && ft_strnstr(env[i - 1], "PATH=", 5) == NULL)
		return (NULL);
	env_paths = ft_split(env[i] + 5, ':');
	return (env_paths);
}

char	*find_bin_in_path(char **env_paths, char *cmd)
{
	char	*current_path;
	bool	check;
	int		i;

	i = 0;
	check = false;
	while (env_paths[i] != NULL)
	{
		current_path = ft_strjoin(env_paths[i], cmd);
		if (access(current_path, X_OK) == -1)
			i++;
		else
		{
			check = true;
			break ;
		}
		free(current_path);
	}
	if (check == false)
		return (NULL);
	return (current_path);
}

char	*get_bin_path(char *cmd, char **env)
{
	char	**env_paths;
	char	**args;
	char	*temp;
	char	*path;

	if (access(cmd, X_OK) != -1)
	{
		path = ft_strdup(cmd);
		return (NULL);
	}
	env_paths = parse_paths(env);
	args = ft_split(cmd, ' ');
	temp = ft_strjoin("/", args[0]);
	path = find_bin_in_path(env_paths, temp);
	free(temp);
	return (path);
}

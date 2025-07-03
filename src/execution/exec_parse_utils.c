/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:35:04 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/03 19:09:57 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_cmd_args(char *cmd, char *path)
{
	char	**args;
	char	**cmd_args;
	int		i;

	args = mini_split(cmd, ' ');
	i = 0;
	while (args[i])
		i++;
	cmd_args = arena_malloc((i + 1) * sizeof(char *));
	cmd_args[0] = mini_strdup(path);
	i = 1;
	while (args[i])
	{
		cmd_args[i] = mini_strdup(args[i]);
		i++;
	}
	cmd_args[i] = NULL;
	return (cmd_args);
}

static char	**parse_paths(char **env)
{
	char	**env_paths;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			env_paths = mini_split(env[i] + 5, ':');
			return (env_paths);
		}
		i++;
	}
	return (NULL);
}

char	*find_bin_in_path(char **env_paths, char *cmd)
{
	char	*current_path;
	int		i;

	i = 0;
	while (env_paths[i] != NULL)
	{
		current_path = mini_join(env_paths[i], cmd);
		if (access(current_path, X_OK) == -1)
		{
			if (open(current_path, O_RDONLY) >= 0)
			{
				ft_fprintf(2, "minishell: %s: Permission denied\n",
					current_path);
				break ;
			}
			i++;
		}
		else
			return (current_path);
	}
	return (NULL);
}

char	*get_bin_path(char *cmd, char **env)
{
	char	**env_paths;
	char	**args;
	char	*temp;
	char	*path;

	if (access(cmd, X_OK) != -1)
	{
		path = mini_strdup(cmd);
		return (NULL);
	}
	env_paths = parse_paths(env);
	if (!env_paths)
		ft_exit(mini_join(MS, mini_join(cmd, NSFOD)), 127);
	args = mini_split(cmd, ' ');
	temp = mini_join("/", args[0]);
	path = find_bin_in_path(env_paths, temp);
	if (!path)
		ft_exit_child(mini_join(cmd, ": command not found"), 127);
	return (path);
}

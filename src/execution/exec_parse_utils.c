/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:35:04 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/13 02:02:28 by jpelline         ###   ########.fr       */
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
		if (ft_strnstr(env[i], "PATH=", 5))
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
	int		fd;
	t_stat	st;

	i = 0;
	while (env_paths[i] != NULL)
	{
		current_path = mini_join(env_paths[i], cmd);
		if (access(current_path, X_OK) == -1)
		{
			fd = open(current_path, O_RDONLY);
			if (fd >= 0)
			{
				close(fd);
				ft_fprintf(2, "minishell: %s: Permission denied\n",
					current_path);
				break ;
			}
		}
		else if (stat(current_path, &st) == 0)
		{
            if (S_ISREG(st.st_mode) && (st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
                return (current_path);
		}
		else
			return (current_path);
		i++;
	}
	return (NULL);
}

static void	check_cmd_validity(t_pipedata *p, char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_isspace(cmd[i]))
		{
			ft_exit_child(p, mini_join(mini_strndup(cmd, ft_strlen(cmd)),
					": command not found"), 127);
		}
		else
			i++;
	}
}

char	*get_bin_path(char *cmd, char **env, t_pipedata *p)
{
	char	**env_paths;
	char	**args;
	char	*temp;
	char	*path;

	if (ft_strncmp(cmd, "./", 2) == 0 && access(cmd, X_OK) == 0)
	{
		cmd = mini_join(get_pwd(), cmd + 1);
		return (cmd);
	}
	open_handler(p, cmd);
	if (access(cmd, X_OK) == 0 && ft_strncmp(cmd, "/", 1) == 0)
		return (mini_strdup(cmd));
	if (path_exists() == -1 && access(cmd, X_OK) == 0)
		return (mini_strdup(cmd));
	env_paths = parse_paths(env);
	if (!env_paths)
		ft_exit_child(p, mini_join(MS, mini_join(mini_strndup(cmd,
						word_len(cmd, 0)), NSFOD)), 127);
	args = mini_split(cmd, ' ');
	check_cmd_validity(p, cmd);
	temp = mini_join("/", args[0]);
	path = find_bin_in_path(env_paths, temp);
	open_handler(p, path);
	if (!path)
		ft_exit_child(p, mini_join(mini_strndup(cmd, word_len(cmd, 0)),
				": command not found"), 127);
	return (path);
}

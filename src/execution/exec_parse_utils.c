/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:35:04 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/18 01:20:26 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	check_file_permissions(char *current_path)
{
	int		fd;
	t_stat	st;

	if (access(current_path, X_OK) == -1)
	{
		fd = open(current_path, O_RDONLY);
		if (fd >= 0)
		{
			close(fd);
			ft_fprintf(2, "minishell: %s: Permission denied\n",
				current_path);
			return (-1);
		}
		return (0);
	}
	else if (stat(current_path, &st) == 0)
	{
		if (S_ISREG(st.st_mode)
			&& (st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
			return (1);
		return (0);
	}
	else
		return (1);
}

char	*find_bin_in_path(char **env_paths, char *cmd)
{
	char	*current_path;
	int		i;
	int		perm_result;

	i = 0;
	while (env_paths[i] != NULL)
	{
		current_path = mini_join(env_paths[i], cmd);
		perm_result = check_file_permissions(current_path);
		if (perm_result == -1)
			break ;
		else if (perm_result == 1)
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
		return (check_dir(p, cmd));
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

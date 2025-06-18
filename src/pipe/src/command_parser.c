/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 13:18:31 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/17 18:34:39 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	get_cmd_args(t_pipex *p, char *av_cmd)
{
	char	**args;
	int		i;

	args = ft_split(av_cmd, ' ');
	if (!args)
		free_exit(p, NULL, NULL, true);
	i = 0;
	while (args[i])
		i++;
	p->cmd_args = ft_calloc(i + 1, sizeof(char *));
	if (!p->cmd_args)
		free_exit(p, args, NULL, true);
	p->cmd_args[0] = ft_strdup(p->path);
	if (!p->cmd_args[0])
		free_exit(p, args, p->cmd_args, true);
	i = 1;
	while (args[i])
	{
		p->cmd_args[i] = ft_strdup(args[i]);
		if (!p->cmd_args[i++])
			free_exit(p, args, p->cmd_args, true);
	}
	p->cmd_args[i] = NULL;
	free_exit(p, args, NULL, false);
}

static char	**parse_paths(char **env)
{
	char	**env_paths;
	int		i;
	int		check;

	check = 0;
	i = 0;
	if (!env)
		return (NULL);
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
	if (!env_paths)
		return (NULL);
	return (env_paths);
}

char	*find_bin_in_path(char **env_paths, char *cmd)
{
	char	*current_path;
	int		i;
	bool	check;

	i = 0;
	check = false;
	while (env_paths[i] != NULL)
	{
		current_path = ft_strjoin(env_paths[i], cmd);
		if (!current_path)
			return (NULL);
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

void	get_bin_path(t_pipex *p, char *av_index, char **env)
{
	char	**env_paths;
	char	**args;
	char	*cmd;

	if (access(av_index, X_OK) != -1)
	{
		p->path = ft_strdup(av_index);
		if (!p->path)
			free_exit(p, NULL, NULL, true);
		return ;
	}
	env_paths = parse_paths(env);
	if (!env_paths)
		exit_handler(127, "Error", p);
	args = ft_split(av_index, ' ');
	if (!args)
		free_exit(p, env_paths, NULL, true);
	cmd = ft_strjoin("/", args[0]);
	if (!cmd)
		free_exit(p, env_paths, args, true);
	p->path = find_bin_in_path(env_paths, cmd);
	free(cmd);
	free_arrays(env_paths, args);
	free_exit(p, NULL, NULL, false);
}

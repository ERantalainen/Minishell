/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:05 by erantala          #+#    #+#             */
/*   Updated: 2025/07/05 04:14:12 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	change_dir(char *path, t_data *data)
{
	size_t	i;

	i = 0;
	while (path[i])
	{
		if (ft_isspace(path[i]))
		{
			ft_fprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
			return ;
		}
		i++;
	}
	if (chdir(path) == -1)
	{
		perror((mini_join("minishell: cd: ", path)));
		replace_export("?=1");
	}
	else
	{
		replace_export(mini_join("OLDPWD=", data->directory));
		data->directory = get_pwd();
		replace_export(mini_join("PWD=", get_pwd()));
		replace_export("?=0");
	}
}

static char	*cd_make_path(t_cmd **cmd, int i)
{
	char	*path;

	path = "";
	if (cmd[i]->next == EMPTY)
		return (cmd[i]->str);
	while(cmd[i] && cmd[i]->type == FILES)
	{
		if(cmd[i]->str[0] == '/')
			path = mini_join(path, cmd[i]->str);
		else
		{
			ft_fprintf(2, "%s\n", mini_join(MS, MARG));
			return (NULL);
		}
		i++;
	}
	return (path);
}

void	cd(t_cmd **cmd, int i)
{
	t_data	*data;
	char	*path;

	data = get_data();

	if ((cmd[i]->next != FILES && cmd[i]->next != STRING)
		&& ft_strlen(cmd[i]->str) == 2)
	{
		path = find_export("HOME");
		if (ft_strcmp(path, "") == 0)
		{
			ft_fprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
			return ;
		}
	}
	else if (cmd[i]->next == FILES && ft_strlen(cmd[i]->str) == 2)
		path = cd_make_path(cmd, i + 1);
	else
	{
		cmd[i]->str += 3;
		path = cmd[i]->str;
	}
	if (path)
		change_dir(path, data);
}

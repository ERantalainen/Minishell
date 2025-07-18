/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:05 by erantala          #+#    #+#             */
/*   Updated: 2025/07/18 15:03:33 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_cd_res(char *path, t_data *data)
{
	if (!pwd_check())
	{
		replace_export(mini_join("OLDPWD=", data->directory));
		data->directory = (mini_join(data->directory, mini_join("/", path)));
		(void)chdir(data->directory);
		replace_export(mini_join("PWD=", data->directory));
		replace_export("?=0");
		return ;
	}
	replace_export(mini_join("OLDPWD=", data->directory));
	data->directory = get_pwd();
	replace_export(mini_join("PWD=", get_pwd()));
	replace_export("?=0");
}

static void	change_dir(char *path, t_data *data)
{
	if (ft_strcmp(path, "") == 0)
	{
		path = find_export("$HOME");
		if (ft_strcmp(path, "") == 0)
		{
			ft_fprintf(2, "minishell: cd: HOME not set\n");
			replace_export("?=1");
			return ;
		}
	}
	if (chdir(path) == -1)
	{
		perror((mini_join("minishell: cd: ", path)));
		replace_export("?=1");
	}
	else
		check_cd_res(path, data);
}

static char	*cd_make_path(t_cmd **cmd, int i)
{
	char	*path;

	while (cmd[i] && (cmd[i]->type == OUTPUT || cmd[i]->type == APPEND))
		i += 2;
	if (cmd[i])
		path = mini_strdup(cmd[i]->str);
	else
		return ("");
	i++;
	while (cmd[i] && (cmd[i]->type == FILES || cmd[i]->type == STRING
			|| cmd[i]->type == OUTPUT || cmd[i]->type == APPEND))
	{
		if (cmd[i]->type == OUTPUT || cmd[i]->type == APPEND)
			i++;
		else if (cmd[i]->space == 0)
			path = mini_join(path, cmd[i]->str);
		else
		{
			ft_fprintf(2, "%s\n", mini_join(MS, MARG));
			replace_export("?=1");
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
	if ((cmd[i]->next == EMPTY || cmd[i]->next == PIPE))
	{
		path = find_export("$HOME");
		if (ft_strcmp(path, "") == 0)
		{
			ft_fprintf(2, "minishell: cd: HOME not set\n");
			replace_export("?=1");
			return ;
		}
	}
	else if ((cmd[i]->next != EMPTY)
		&& ft_strlen(cmd[i]->str) == 2)
		path = cd_make_path(cmd, i + 1);
	else
	{
		cmd[i]->str += 3;
		path = cmd[i]->str;
	}
	if (path)
		change_dir(path, data);
}

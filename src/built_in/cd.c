/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:05 by erantala          #+#    #+#             */
/*   Updated: 2025/07/02 00:46:57 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	change_dir(char *path, t_data *data)
{
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
	else if (cmd[i]->next == FILES)
		path = cmd[i + 1]->str;
	else
	{
		cmd[i]->str += 3;
		path = cmd[i]->str;
	}
	change_dir(path, data);
}

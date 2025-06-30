/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:05 by erantala          #+#    #+#             */
/*   Updated: 2025/06/30 20:15:45 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd(t_cmd **cmd, int i)
{
	t_data	*data;
	char	*path;
	data = get_data();


	if ((cmd[i]->next != FILES || cmd[i]->next != STRING)
		&& ft_strlen(cmd[i]->str) == 2)
			path = find_export("HOME");
	else if (cmd[i]->next == FILES)
			path = cmd[i + 1]->str;
	else
		path = cmd[i]->str += 3;
	if (chdir(path) == -1)
	{
		perror((mini_join("minishell: cd: ", path)));
		replace_export("?=1");
	}
	else
	{
		data->directory = get_pwd();
		replace_export(mini_join("PWD=", get_pwd()));
		replace_export("?=0");
	}
}

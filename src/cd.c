/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:05 by erantala          #+#    #+#             */
/*   Updated: 2025/06/26 03:04:22 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd(t_cmd *cmd)
{
	t_data	*data;
	char	*path;
	data = get_data();

	if (cmd == NULL)
		path = find_export("HOME");
	else
		path = cmd->str;
	if (chdir(path) == -1)
	{
		if (errno == ENOENT)
			printf("File does not exist\n");
		if (errno == EACCES)
			printf("Permission deniend\n");
	}
	data->directory = get_pwd();
}
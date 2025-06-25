/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:05 by erantala          #+#    #+#             */
/*   Updated: 2025/06/25 17:58:32 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd(t_cmd *cmd)
{
	t_data	*data;
	char	*path;
	data = get_data();

	path = cmd->str;
	printf("Path: %s", path);
	if (chdir(path) == -1)
	{
		if (errno == ENOENT)
			printf("File does not exist\n");
		if (errno == EACCES)
			printf("Permission deniend\n", EACCES);
	}
	data->directory = get_pwd();
}
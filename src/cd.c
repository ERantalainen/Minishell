/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:05 by erantala          #+#    #+#             */
/*   Updated: 2025/06/25 00:30:31 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd(char *path)
{
	t_data	*data;

	data = get_data();
	if (chdir(path) == -1)
	{
		if (errno == ENOENT)
			ft_exit("File does not exist", ENOENT);
		if (errno == EACCES)
			ft_exit("Permission deniend", EACCES);
	}
	data->directory = get_pwd();
}
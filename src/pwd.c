/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:23:27 by erantala          #+#    #+#             */
/*   Updated: 2025/06/24 18:32:12 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(void)
{
	char	*path;
	int		len;

	len = 100;
	while (1)
	{
		path = arena_malloc(len);
		path = getcwd(path, 100);
		if (!path && errno == ERANGE)
			len += 25;
		else if (!path)
			ft_exit("Can't get path", errno);
		else
		{
			ft_putendl_fd(path, 1);
			break ;
		}
	}
}
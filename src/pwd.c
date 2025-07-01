/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:23:27 by erantala          #+#    #+#             */
/*   Updated: 2025/07/01 02:57:39 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(void)
{
	char *output;

	output = get_pwd();
	if (output == NULL)
		make_export("?=1");
	else
		ft_putendl_fd(output, 1);
}


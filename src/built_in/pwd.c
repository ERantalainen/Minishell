/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:23:27 by erantala          #+#    #+#             */
/*   Updated: 2025/07/10 15:49:58 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(void)
{
	char	*output;

	output = get_pwd();
	if (output == NULL)
		replace_export("?=1");
	else
		ft_putendl_fd(output, 1);
	replace_export("?=0");
}

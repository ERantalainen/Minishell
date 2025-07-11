/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utility_extra.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:59:20 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/11 22:07:15 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	path_exists(void)
{
	char	*path;

	path = find_export("PATH");
	if (ft_strcmp(path, "") == 0)
		return (-1);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:53:35 by erantala          #+#    #+#             */
/*   Updated: 2025/07/07 18:15:38 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_input(char **argv, int argc)
{
	static int	pos = 0;

	pos++;
	if (pos < argc)
		return (argv[pos]);
	else
		return (NULL);
}

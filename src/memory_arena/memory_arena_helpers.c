/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_arena_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 21:26:37 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/11 22:08:34 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arenas(void)
{
	t_arena	**arenas;
	int		i;

	i = 0;
	arenas = get_arenas(NULL);
	while (arenas[i])
	{
		free(arenas[i]);
		i++;
	}
	free(arenas);
}

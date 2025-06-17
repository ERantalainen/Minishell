/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:05:32 by erantala          #+#    #+#             */
/*   Updated: 2025/06/17 16:56:03 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

# define ARENA_SIZE 16384
# define ALIGNMENT 8

typedef struct s_arena
{
	size_t	size;
	size_t	max;
	size_t	index;
	char	data[];
}	t_arena;

void	*arena_malloc(size_t n);

#endif
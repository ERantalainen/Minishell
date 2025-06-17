/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_arena.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:05:04 by erantala          #+#    #+#             */
/*   Updated: 2025/06/18 02:06:18 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

// Memory arena to store all allocated memory in one location.

void	*arena_malloc(size_t n)
{
	t_arena	*arena;
	void	*ret;
	size_t	alg_i;

	arena = get_arenas(n);
	alg_i = (arena->index + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
	arena->size += n;
	ret = &arena->data[alg_i];
	arena->index = alg_i + n + 1;
	return (ret);
}

// gets the next available arena and returns a pointer to the free memory

t_arena *init_arena(size_t size)
{
	t_arena	*arena;

	arena = ft_calloc(size, 1);
	if (!arena)
		exit(1);
	arena->max = size;
	arena->size = 0;
	arena->index = 0;
	return (arena);
}

// Returns a free point in the arena, if arenas are full allocates more.

t_arena *get_arenas(size_t n)
{
	static int		arena_count = 0;
	static int		index = 0;
	static t_arena **arenas;

	if (arena_count < 2)
	{
		arenas = malloc(sizeof(t_arena *) * 2);
		if (!arenas)
			exit;
		arenas[arena_count++] = init_arena(ARENA_SIZE);
		arenas[arena_count++] = init_arena(ARENA_SIZE);
	}
	while (index < arena_count && arenas[index]->max - arenas[index]->size < n)
	while (arenas[index]->max - arenas[index]->size < n && index < arena_count)
		index++;
	if (index == arena_count)
	{
		arenas = new_arena(arenas, arena_count, n);
		index++;
	}
	return (arenas[index]);
}

// Creates a new arena when there isn't enough memory in current arenas.

t_arena **new_arena(t_arena **curr, int	count, size_t n)
{
	t_arena	**arenas;
	int		i = 0;

	arenas = malloc(sizeof(t_arena *) * count + 1);
	if (!arenas)
		exit(1);
	while (i < count)
	{
		arenas[i] = curr[i];
		i++;
	}
	arenas[i] = init_arena(n);
	return (arenas);
}

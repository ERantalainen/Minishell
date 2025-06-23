/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:48:14 by erantala          #+#    #+#             */
/*   Updated: 2025/06/23 18:19:47 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_vector	*new_vector(size_t elem)
{
	t_vector	*vector;

	vector = arena_malloc(sizeof(t_vector));
	if (!vector)
		exit(1);
	vector->data = arena_malloc(sizeof(void *) * (elem * 2 + 1));
	vector->count = 0;
	vector->size = elem * 2;
	vector->data[elem * 2] = NULL;
	return (vector);
}

// Creates a new vector with elem * 2 space.

t_vector	*expand_vector(t_vector *vector, size_t elems)
{
	t_vector	*dup;
	size_t		i;

	i = 0;
	dup = new_vector(elems);
	while(i < vector->count && vector->data[i] != NULL)
	{
		dup->data[i] = vector->data[i];
		i++;
	}
	dup->count = vector->count;
	return (dup);
}

// Expands a vector to add space for more elements.

int	add_elem(t_vector *vector, void *elem)
{
	if (vector->count == vector->size)
		vector = expand_vector(vector, vector->count + 1);
	vector->data[vector->count] = elem;
	vector->count++;
	return (vector->count);
}

// Adds a new element to the end of the vector and expands the vector if needed.

void	change_data(t_vector *vector, void *elem, void *target)
{
	size_t	i;

	i = 0;
	while (i < vector->count && vector->data[i] != NULL)
	{
		if (vector->data[i] == target)
		{
			vector->data[i] = elem;
			return ;
		}
		i++;
	}
}

// Finds element target and replaces it with elem in the designated vector.
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:48:14 by erantala          #+#    #+#             */
/*   Updated: 2025/06/26 18:48:04 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_vector	*new_vector(size_t elem)
{
	t_vector	*vector;

	vector = arena_malloc(sizeof(t_vector));
	if (!vector)
		exit(1);
	vector->data = arena_malloc(sizeof(void *) * (elem * 2));
	vector->count = 0;
	vector->size = elem * 2;
	return (vector);
}

// Creates a new vector with elem * 2 space.

void	expand_vector(t_vector *vector)
{
	void	**new;
	size_t	i;

	i = 0;
	new = arena_malloc(sizeof(void *) * (vector->size * 2 + 1));
	while (i < vector->count)
	{
		new[i] = vector->data[i];
		i++;
	}
	vector->data = new;
	vector->size *= 2;
	vector->data[vector->size] = NULL;
}

// Expands a vector to add space for more elements.

void	add_elem(t_vector *vector, void *elem)
{
	if (vector->count == vector->size - 1)
		expand_vector(vector);
	vector->data[vector->count] = elem;
	vector->count++;
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

void	array_to_vec(t_vector *vec, void **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
	{
		add_elem(vec, arr[i]);
		i++;
	}
}

char	**vec_to_array(t_vector *vec)
{
	size_t	i;
	char	**res;
	char	*str;

	i = 0;
	res = arena_malloc(sizeof(char *) * (vec->count + 1));
	while (i < vec->count)
	{
		str = vec->data[i];
		res[i] = str;
		i++;
	}
	res[i] = NULL;
	return (res);
}

// Finds element target and replaces it with elem in the designated vector.

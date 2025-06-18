/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 16:48:14 by erantala          #+#    #+#             */
/*   Updated: 2025/06/18 17:27:35 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_vector	*new_vector(size_t elem)
{
	t_vector	*vector;

	vector = arena_malloc(sizeof(vector));
	if (!vector)
		exit(1);
	vector->data = arena_malloc(sizeof(void *) * (elem * 2 + 1));
	vector->count = 0;
	vector->size = elem * 2;
	vector->data[elem * 2] = NULL;

	return (vector);
}

t_vector	*expand_vector(t_vector *vector, size_t elems)
{
	t_vector	*dup;
	int			i;

	i = 0;
	dup = new_vector(elems);
	while(i < vector->count && vector->data[i] != NULL)
	{
		dup->data[i] = vector->data[i];
		i++;
	}
	return (dup);
}

int	add_elem(t_vector *vector, void *elem)
{
	if (vector->count == vector->size)
		expand_vector(vector, vector->count + 1);
	vector->data[vector->count + 1] = elem;
	vector->count++;
	return (vector->count);
}

void	change_data(t_vector *vector, void *elem, void *target)
{
	int	i;

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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:20:21 by erantala          #+#    #+#             */
/*   Updated: 2025/07/10 21:25:57 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_elem(t_vector *vector, size_t i)
{
	while (i < vector->count - 1)
	{
		vector->data[i] = vector->data[i + 1];
		i++;
	}
	if (i == vector->count - 1)
	{
		vector->data[i] = NULL;
		vector->count--;
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

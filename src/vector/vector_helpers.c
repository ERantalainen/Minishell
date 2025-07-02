/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:20:21 by erantala          #+#    #+#             */
/*   Updated: 2025/06/26 15:26:23 by erantala         ###   ########.fr       */
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

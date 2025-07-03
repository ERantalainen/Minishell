/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_dup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 22:34:19 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/03 22:34:39 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**array_dup(char **array)
{
	size_t	count;
	size_t	i;
	char	**copy;

	count = 0;
	while (array[count])
		count++;
	copy = malloc((count + 1) * sizeof(char *));
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(array[i]);
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

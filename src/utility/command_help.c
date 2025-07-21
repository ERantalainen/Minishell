/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:33:04 by erantala          #+#    #+#             */
/*   Updated: 2025/07/21 14:26:24 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_vector	*next_check(t_vector *commands)
{
	t_cmd	*curr;
	t_cmd	*next;
	size_t	i;

	i = 0;
	while (i < commands->count - 1)
	{
		curr = commands->data[i];
		next = commands->data[i + 1];
		if (curr->type == HERE_DOC)
			next->type = FILES;
		curr->next = next->type;
		i++;
	}
	return (commands);
}

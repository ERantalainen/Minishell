/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:48:23 by erantala          #+#    #+#             */
/*   Updated: 2025/06/19 16:33:17 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_repeat(t_vector *tokens)
{
	int		i;
	t_token	*token_curr;
	t_token	*token_next;

	i = 0;
	while (i < tokens->count - 1)
	{
		token_curr = tokens->data[i];
		token_next = tokens->data[i + 1];;
		if (token_curr->t == PIPE && token_next->t == PIPE)
			exit(1);
		
	}
}
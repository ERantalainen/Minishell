/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utility.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:03:38 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/16 22:39:49 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	skip_redirects(t_cmd **tokens, size_t tok_i)
{
	while (tokens[tok_i] && (tokens[tok_i]->type == OUTPUT
			|| tokens[tok_i]->type == APPEND
			|| tokens[tok_i]->type == HERE_DOC
			|| tokens[tok_i]->type == INPUT))
	{
		tok_i++;
		if (tokens[tok_i] && (tokens[tok_i]->type == FILES))
		{
			tok_i++;
			continue ;
		}
	}
	return (tok_i);
}

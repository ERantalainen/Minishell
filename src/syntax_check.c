/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:48:23 by erantala          #+#    #+#             */
/*   Updated: 2025/06/23 14:44:16 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_repeat(t_vector *tokens)
{
	size_t		i;
	t_token	*token_curr;
	t_token	*token_next;

	i = 0;
	while (i < tokens->count - 1)
	{
		token_curr = tokens->data[i];
		token_next = tokens->data[i + 1];
		if (token_curr->t == PIPE && token_next->t == PIPE)
			exit(1);
		if (token_curr->t == INPUT && token_next->t != STRING)
			exit(1);
		if (token_curr->t == OUTPUT && token_next->t != STRING)
			exit(1);
		i++;
	}
}

// ENSURE NO SYNTAX ERROR WITH REPEAT SYMBOLS

// int	check_heredoc(t_vector	*tokens)
// {
// 	size_t	i;
// 	size_t	count;
// 	t_token	*curr;
// 	t_token	*next;

// 	while (i < tokens->count - 1)
// 	{
// 		curr = tokens->data[i];
// 		next = tokens->data[i];
// 		if (curr->t == HERE_DOC)
// 		{
// 			if (next->t != STRING)
// 				exit(1);
// 			if (ft_strlen(curr->s) > 2)
// 				exit(1);
// 			count++;
// 		}
// 		i++;
// 	}
// 	if (count > 16)
// 		exit(1);
// 	if (count > 0)
// 		here_doc(tokens);
// }

// // CHECK heredoc validity by ensuring a delimiter found after every heredoc
// // and heredoc count.

// char	*here_doc(t_vector	*tokens)
// {
// 	// TAKE HEREDOC INPUT
// }
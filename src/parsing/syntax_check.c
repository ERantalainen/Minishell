/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:48:23 by erantala          #+#    #+#             */
/*   Updated: 2025/07/02 17:58:42 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_repeat(t_vector *tokens)
{
	size_t	i;
	t_token	*cur;
	t_token	*nx;
	t_data	*data;

	data = get_data();
	i = 0;
	while (i < tokens->count - 1)
	{
		cur = tokens->data[i];
		nx = tokens->data[i + 1];
		if (cur->t == PIPE && nx->t == PIPE)
		{
			data->valid = 0;
			ft_fprintf(STDERR_FILENO, "%s\n", mini_join(TOKEN, "|'"));
		}
		if (cur->t == INPUT && nx->t != STRING && nx->t != FILES)
			data->valid = 0;
		if (cur->t == OUTPUT && nx->t != STRING && nx->t != FILES)
			data->valid = 0;
		if (data->valid == 0)
			return ;
		i++;
	}
}

// ENSURE NO SYNTAX ERROR WITH REPEAT SYMBOLS
int	check_heredoc(t_vector *tokens)
{
	size_t	i;
	size_t	count;
	t_token	*curr;
	t_token	*next;
	t_data	*data;

	data = get_data();
	count = 0;
	i = 0;
	while (i < tokens->count - 1)
	{
		curr = tokens->data[i];
		next = tokens->data[i + 1];
		if (curr->t == HERE_DOC)
		{
			if (next->t != STRING && next->t != INPUT && next->t != HERE_NOEXP)
				exit(1);
			if (next->t == INPUT)
				return (0);
			count++;
		}
		i++;
	}
	if (count > 16)
		exit(5);
	if (count > 0)
	{
		data->hd_count = count;
		data->hdfd = arena_malloc(sizeof(int) * count);
	}
	i = 0;
	while (count > 0)
	{
		curr = tokens->data[i];
		next = tokens->data[i + 1];
		if ((curr->t == HERE_DOC) && next != NULL)
		{
			next->s = here_doc(next->s, count - 1, next->t);
			if (!data->valid)
				return (0);
			next->t = FILES;
			count--;
		}
		i++;
	}
	return (0);
}

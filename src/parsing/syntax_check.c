/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:48:23 by erantala          #+#    #+#             */
/*   Updated: 2025/07/09 02:36:35 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	er_pr(char *error, t_data *data, int exit, bool per)
{
	if (per == 1)
		perror(error);
	else
		ft_fprintf(2, "%s\n", error);
	data->valid = 0;
	replace_export(mini_join("?=", mini_itoa(exit)));
}

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
		if (cur->t == INPUT && nx->t == INPUT)
		{
			remove_elem(tokens, i);
			nx->t = HERE_DOC;
			nx->s = mini_strdup("<<");
		}
		if (cur->t == PIPE && nx->t == PIPE)
			er_pr(mini_join(TOKEN, "|'"), data, 127, 0);
		if (data->valid == 0)
			return ;
		i++;
	}
}

void	check_nexts(t_vector *tokens, t_data *data)
{
	t_token	*curr;
	t_token	*next;
	size_t	i;

	i = 0;
	while (i < tokens->count)
	{
		curr = tokens->data[i];
		next = tokens->data[i + 1];
		if (!next && curr->t == HERE_DOC)
		{
			er_pr(mini_join(TOKEN, "newline\'"), data, 2, 0);
			return ;
		}
		if (curr->t == HERE_DOC)
		{
			if (next->t != STRING && next->t != HERE_NOEXP)
			{
				er_pr(mini_join(TOKEN, mini_join(next->s, "\'")), data, 2, 0);
				return ;
			}
		}
		i++;
	}	
}

// ENSURE NO SYNTAX ERROR WITH REPEAT SYMBOLS
int	check_heredoc(t_vector *tokens)
{
	int	count;
	t_token	*curr;
	t_data	*data;

	data = get_data();
	curr = tokens->data[0];
	if (tokens->count == 1 && curr->t == HERE_DOC)
	{
		er_pr(mini_join(TOKEN, "newline\'"), data, 2, 0);
		return (0);
	}
	check_nexts(tokens, data);
	if (!data->valid)
		return (0);
	count = here_count(tokens, data);
	if (count > 16)
		ft_exit("minishell: here document count exceeded", 2);
	if (count > 0)
	{
		data->hd_count = count;
		data->hdfd = arena_malloc(sizeof(int) * count);
		here_two(tokens, count, data);
	}
	return (0);
}

int	here_count(t_vector *tokens, t_data *data)
{
	t_token	*curr;
	t_token	*next;
	size_t	i;
	size_t	count;

	count = 0;
	i = 0;
	while (i < tokens->count - 1)
	{
		curr = tokens->data[i];
		next = tokens->data[i + 1];
		if (curr->t == HERE_DOC)
		{
			if (next->t != STRING && next->t != HERE_NOEXP)
			{
				er_pr(mini_join(TOKEN, mini_join(next->s, "\'")), data, 2, 0);
				return (-1);
			}
			count++;
		}
		i++;
	}
	return (count);
}
void	here_two(t_vector *tokens, int count, t_data *data)
{
	t_token	*curr;
	t_token	*next;
	size_t	i;

	i = 0;
	while (count > 0)
	{
		curr = tokens->data[i];
		next = tokens->data[i + 1];
		if ((curr->t == HERE_DOC) && next != NULL)
		{
			next->s = here_doc(next->s, count - 1, next->t);
			if (!data->valid)
				return ;
			next->t = FILES;
			remove_elem(tokens, i);
			count--;
		}
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:47:36 by erantala          #+#    #+#             */
/*   Updated: 2025/07/21 18:23:48 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_here_prompt(t_vector *vec, int i, int count)
{
	char	*limiter;
	t_type	here_type;
	t_token	**tokens;
	int		j;

	j = i + 1;
	tokens = (t_token **)vec->data;
	here_type = tokens[j]->t;
	limiter = mini_strdup(tokens[j]->s);
	if (tokens[j]->quoted == 1)
		here_type = HERE_NOEXP;
	j++;
	while (tokens[j] && (tokens[j]->t == HERE_NOEXP
			|| tokens[j]->t == STRING) && (!tokens[j]->space))
	{
		if (tokens[j]->quoted == 1)
			here_type = HERE_NOEXP;
		while (tokens[j] && (!tokens[j]->space || j == i))
		{
			limiter = mini_join(limiter, tokens[j]->s);
			j++;
		}
		remove_elem(vec, j - 1);
	}
	return (here_doc(limiter, count - 1, here_type));
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
			next->s = create_here_prompt(tokens, i, count);
			if (!data->valid)
				return ;
			next->t = FILES;
			count--;
		}
		i++;
	}
}

void	check_command_syntax(t_vector *commands, t_data *data, int i)
{
	size_t	i;
	t_cmd	*cmd;

	cmd = commands->data[i];
	if (commands->data[i + 1]
		&& (cmd->next == STRING || cmd->next == FILES))
		check_files(cmd, commands->data[i + 1]);
	if (data->valid != 1)
		return ;
	syntax_help(cmd, data, i, commands);
	if (data->valid != 1)
	{
		replace_export("?=2");
		return ;
	}
	i++;
}

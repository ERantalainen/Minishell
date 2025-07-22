/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:47:36 by erantala          #+#    #+#             */
/*   Updated: 2025/07/22 04:41:52 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_here_prompt(t_vector *vec, int i, int count)
{
	char	*limiter;
	t_type	here_type;
	t_cmd	**tks;
	int		j;

	j = i + 1;
	tks = (t_cmd **)vec->data;
	here_type = tks[j]->type;
	limiter = mini_strdup(tks[j]->str);
	if (tks[j]->quoted == 1)
		here_type = HERE_NOEXP;
	j++;
	while (tks[j] && (tks[j]->type == HERE_NOEXP || tks[j]->type == FILES
			|| tks[j]->type == STRING) && (!tks[j]->space))
	{
		if (tks[j]->quoted == 1)
			here_type = HERE_NOEXP;
		while (tks[j] && (!tks[j]->space || j == i))
		{
			limiter = mini_join(limiter, tks[j]->str);
			j++;
		}
		remove_elem(vec, j - 1);
	}
	return (here_doc(limiter, count - 1, here_type));
}

void	here_two(t_vector *tokens, int count, t_data *data)
{
	t_cmd	*curr;
	int		i;

	i = 0;
	while (count > 0 && tokens->data[i])
	{
		curr = tokens->data[i];
		if (!data->valid)
			return ;
		if ((curr->type == HERE_DOC) && curr->next != EMPTY)
		{

			curr->str = create_here_prompt(tokens, i, count);
			if (!data->valid)
				return ;
			curr->type = FILES;
			count--;
		}
		check_command_syntax(tokens, data, i);
		if (!data->valid)
			break ;
		i++;
	}
}

void	check_command_syntax(t_vector *commands, t_data *data, int i)
{
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
}

void	check_all_syntax(t_vector *commands, t_data *data)
{
	t_cmd	*cmd;
	size_t	i;

	i = 0;

	while (i < commands->count)
	{
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
}
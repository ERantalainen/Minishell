/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:40:32 by erantala          #+#    #+#             */
/*   Updated: 2025/07/07 18:41:46 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	input_syntax(t_cmd *cmd, t_data *data)
{
	if (cmd->type == INPUT && cmd->next == EMPTY)
			data->valid = -1;
	else if (cmd->type == INPUT && cmd->next != FILES)
			data->valid = -cmd->next;
}

static void output_syntax(t_cmd *cmd, t_data *data)
{
		if (cmd->type == OUTPUT && cmd->next == EMPTY)
			data->valid = -1;
		else if (cmd->type == OUTPUT && cmd->next != STRING
			&& cmd->next != FILES)
			data->valid = -cmd->next;
}
static void appen_syntax(t_cmd *cmd, t_data *data)
{
	if (cmd->type == APPEND && cmd->next == EMPTY)
		data->valid = -1;
	else if (cmd->type == APPEND && cmd->next != STRING
		&& cmd->next != FILES)
		data->valid = -cmd->next;
}

static void	check_files(t_cmd *cmd, t_cmd *next, t_data *data)
{
	if (cmd->type == INPUT)
	{
		if (access(next->str, R_OK) != 0)
		{
			syntax_print(mini_join(MS, mini_strndup(next->str, word_len(next->str, 0))), data, 2, 1);
			return ;
		}
		cmd->next = FILES;
		next->type = FILES;
	}
	if (cmd->type == OUTPUT)
	{
		cmd->next = FILES;
		next->type = FILES;
	}
}

void	check_command_syntax(t_vector *commands, t_data *data)
{
	size_t	i;
	t_cmd	*cmd;

	i = 0;
	while (i < commands->count)
	{
		cmd = commands->data[i];
		if (commands->data[i + 1] && (cmd->next == STRING || cmd->next == FILES))
			check_files(cmd, commands->data[i + 1], data);
		if (data->valid != 1)
			return ;
		input_syntax(cmd, data);
		output_syntax(cmd, data);
		appen_syntax(cmd, data);
		if (data->valid == -1)
			ft_fprintf(2, "%s'\n", mini_join(TOKEN, "newline"));
		else if (data->valid != 1 && data->valid != -1)
		{
			cmd = commands->data[i + 1];
			ft_fprintf(2, "%s'\n", mini_join(TOKEN, cmd->str));
		}
		if (data->valid != 1)
			return ;
		i++;
	}
}
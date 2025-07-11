/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:40:32 by erantala          #+#    #+#             */
/*   Updated: 2025/07/11 15:17:46 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	input_syntax(t_cmd *cmd, t_data *data)
{
	if (cmd->type == INPUT && cmd->next == EMPTY)
		data->valid = -10;
	else if (cmd->type == INPUT && cmd->next != FILES && cmd->next != STRING)
		data->valid = -1;
}

static void	output_syntax(t_cmd *cmd, t_data *data)
{
	if (cmd->type == OUTPUT && cmd->next == EMPTY)
		data->valid = -10;
	else if (cmd->type == OUTPUT && cmd->next != STRING
		&& cmd->next != FILES)
		data->valid = -1;
}

static void	additional_syntax(t_cmd *cmd, t_data *data, size_t i)
{
	if (cmd->type == APPEND && cmd->next == EMPTY)
		data->valid = -10;
	else if (cmd->type == APPEND && cmd->next != STRING
		&& cmd->next != FILES)
		data->valid = -1;
	if (cmd->type == PIPE && cmd->next == EMPTY)
		data->valid = -10;
	if (cmd->type == PIPE && i == 0)
		er_pr(mini_join(TOKEN, "|'"), data, 2, 0);
}

static void	check_files(t_cmd *cmd, t_cmd *next, t_data *data)
{
	if (cmd->type == INPUT)
	{
		cmd->next = FILES;
		next->type = FILES;
	}
	if (cmd->type == OUTPUT)
	{
		cmd->next = FILES;
		next->type = FILES;
	}
	if (cmd->type == APPEND)
	{
		cmd->next = FILES;
		next->type = FILES;
	}
}

static void	syntax_help(t_cmd *cmd, t_data *data, int i, t_vector *commands)
{
	input_syntax(cmd, data);
	output_syntax(cmd, data);
	additional_syntax(cmd, data, i);
	if (data->valid == -10)
		ft_fprintf(2, "%s'\n", mini_join(TOKEN, "newline"));
	else if (data->valid != 1 && data->valid != 0 && commands->data[i + 1])
	{
		cmd = commands->data[i + 1];
		ft_fprintf(2, "%s'\n", mini_join(TOKEN, cmd->str));
	}
}

void	check_command_syntax(t_vector *commands, t_data *data)
{
	size_t	i;
	t_cmd	*cmd;

	i = 0;
	while (commands && i < commands->count)
	{
		cmd = commands->data[i];
		if (commands->data[i + 1]
			&& (cmd->next == STRING || cmd->next == FILES))
			check_files(cmd, commands->data[i + 1], data);
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

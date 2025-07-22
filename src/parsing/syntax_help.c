/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:40:32 by erantala          #+#    #+#             */
/*   Updated: 2025/07/22 04:41:12 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	input_syntax(t_cmd *cmd, t_data *data)
{
	if (cmd->type == INPUT && cmd->next == EMPTY)
		data->valid = -10;
	else if (cmd->type == INPUT && cmd->next != FILES && cmd->next != STRING
		&& cmd->next != BUILTIN)
		data->valid = -1;
}

static void	output_syntax(t_cmd *cmd, t_data *data)
{
	if (cmd->type == OUTPUT && cmd->next == EMPTY)
		data->valid = -10;
	else if (cmd->type == OUTPUT && cmd->next != STRING
		&& cmd->next != FILES && cmd->next != BUILTIN)
		data->valid = -1;
}

static void	additional_syntax(t_cmd *cmd, t_data *data, size_t i)
{
	if (cmd->type == APPEND && cmd->next == EMPTY)
		data->valid = -10;
	else if (cmd->type == APPEND && cmd->next != STRING
		&& cmd->next != FILES && cmd->next != BUILTIN)
		data->valid = -1;
	if (cmd->type == PIPE && cmd->next == EMPTY)
		data->valid = -10;
	if (cmd->type == PIPE && i == 0)
		er_pr(mini_join(TOKEN, "|'"), data, 2, 0);
	if (cmd->type == PIPE && cmd->next == PIPE)
		data->valid = -1;
}

void	check_files(t_cmd *cmd, t_cmd *next)
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

int	syntax_help(t_cmd *cmd, t_data *data, int i, t_vector *commands)
{
	input_syntax(cmd, data);
	output_syntax(cmd, data);
	additional_syntax(cmd, data, i);
	if (data->valid == -10)
	{
		ft_fprintf(2, "%s'\n", mini_join(TOKEN, "newline"));
		return (-1);
	}
	else if (data->valid != 1 && data->valid != 0 && commands->data[i + 1])
	{
		cmd = commands->data[i + 1];
		ft_fprintf(2, "%s'\n", mini_join(TOKEN, cmd->str));
		return (-1);
	}
	return (0);
}

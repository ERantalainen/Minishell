/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:01 by erantala          #+#    #+#             */
/*   Updated: 2025/07/12 13:36:30 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	combine_arguments(t_cmd **cmd, char **command, int i)
{
	while (cmd[i] && (cmd[i]->type == FILES || cmd[i]->type == STRING))
	{
		if (cmd[i]->space)
			(*command) = mini_append((*command), cmd[i]->str);
		else
			(*command) = mini_join((*command), cmd[i]->str);
		i++;
	}
}

static void	add_to_command(t_cmd **cmd, char **command, int *i)
{
	(*i)++;
	if (!cmd[*i])
		return ;
	(*command) = mini_strdup(cmd[*i]->str);
	(*i)++;
}

static void	check_arg_validity_and_add(t_cmd **cmd, char **command, int *i)
{
	if (ft_strcmp(cmd[*i]->str, "") == 0)
		(*i)++;
	else
		add_to_command(cmd, command, i);
}

static void	process_echo_arguments(t_cmd **cmd, char **command,
					bool *newline, int *i)
{
	char	*part;
	int		pos;

	while (cmd[*i] && (cmd[*i]->type == FILES || cmd[*i]->type == STRING))
	{
		pos = 0;
		part = echo_part(cmd[*i], &pos, newline);
		if (ft_strcmp(part, "") == 0 && cmd[*i]->quoted == 0)
			(*i)++;
		else if (ft_strcmp(part, "") == 0 && cmd[*i]->space && cmd[*i]->quoted)
		{
			check_arg_validity_and_add(cmd, command, i);
			break ;
		}
		else
		{
			(*command) = mini_strdup(part);
			(*i)++;
			break ;
		}
	}
}

void	echo(t_cmd **cmd, int i)
{
	char	*command;
	bool	newline;

	if (!cmd[i])
	{
		if (write(1, "\n", 1) < 0)
			perror("write");
		return ;
	}
	newline = 1;
	command = "";
	process_echo_arguments(cmd, &command, &newline, &i);
	combine_arguments(cmd, &command, i);
	if (newline == 1)
		ft_putendl_fd((mini_strndup(command, ft_strlen(command))), 1);
	else
		ft_putstr_fd((mini_strndup(command, ft_strlen(command))), 1);
	replace_export("?=0");
}

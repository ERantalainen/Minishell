/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:56:30 by erantala          #+#    #+#             */
/*   Updated: 2025/07/11 02:03:19 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	built_in(t_cmd *cmd)
{
	size_t		i;
	const char	*built[8] = {"echo", "cd", "pwd", "export", "unset", "env",
		"exit", NULL};
	size_t		len;

	i = 0;
	if (cmd->type != STRING && cmd->type != FILES)
		return ;
	while (i < 7)
	{
		len = ft_strlen(built[i]);
		if (len < ft_strlen(cmd->str))
			len = ft_strlen(cmd->str);
		if (ft_strncmp(cmd->str, built[i], len) == 0)
			cmd->type = BUILTIN;
		i++;
	}
}

void	build_handler(t_cmd **cmds, int i)
{
	while (cmds[i])
	{
		if (cmds[i]->type == BUILTIN)
		{
			if (ft_strncmp("echo", cmds[i]->str, 4) == 0)
				echo(cmds, i + 1);
			if (ft_strncmp("exit", cmds[i]->str, 4) == 0)
				exit_arg_checker(exit_join(cmds, i));
			if (ft_strncmp("cd", cmds[i]->str, 2) == 0)
				cd(cmds, i);
			if (ft_strncmp("pwd", cmds[i]->str, 3) == 0)
				pwd();
			if (ft_strncmp("export", cmds[i]->str, 6) == 0)
				count_export(cmds, i);
			if (ft_strncmp("unset", cmds[i]->str, 5) == 0)
				count_unset(cmds, i + 1);
			if (ft_strncmp("env", cmds[i]->str, 3) == 0)
				env();
			return ;
		}
		i++;
	}
}

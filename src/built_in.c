/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:56:30 by erantala          #+#    #+#             */
/*   Updated: 2025/06/25 21:22:43 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	built_in(t_cmd *cmd)
{
	size_t		i;
	const	char	*built[8] = { "echo", "cd", "pwd", "export", "unset",
	"env", "exit", NULL};

	i = 0;
	if (cmd->type != STRING)
		return ;
	while (i < 7)
	{
		if (ft_strncmp(cmd->str, built[i], word_len(cmd->str)) == 0)
			cmd->type = BUILTIN;
		i++;
	}
}

void	build_handler(t_vector	*cmds)
{
	size_t		i;
	t_cmd	*cmd;

	i = 0;
	while (i < cmds->count)
	{
		cmd = cmds->data[i];
		if (cmd->type == BUILTIN)
		{
			if (ft_strncmp("echo", cmd->str, 4) == 0)
				echo(cmds, i);
			if (ft_strncmp("exit", cmd->str, 4) == 0)
				ft_exit("exit", 0);
			if (ft_strncmp("cd", cmd->str, 2) == 0)
				cd(cmds->data[i + 1]);
			if (ft_strncmp("pwd", cmd->str, 3) == 0)
				pwd();
			if (ft_strncmp("export", cmd->str, 6) == 0)
				make_export(cmds, i);
			if (ft_strncmp("unset", cmd->str, 5) == 0)
				unset(cmd->str + 6);
			if (ft_strncmp("env", cmd->str, 3) == 0)
				env();
		}
		i++;
	}
}

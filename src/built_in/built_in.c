/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:56:30 by erantala          #+#    #+#             */
/*   Updated: 2025/07/18 14:43:03 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	built_in(t_cmd *cmd, int pos)
{
	size_t		i;
	const char	*built[8] = {"echo", "cd", "pwd", "export", "unset", "env",
		"exit", NULL};
	size_t		len;
	char		*compare;

	i = 0;
	if (cmd->type != STRING && cmd->type != FILES)
		return ;
	compare = cmd->str;
	if (cmd->quoted == 0 && pos == 0)
		compare = mini_strndup(cmd->str, word_len(cmd->str, 0));
	while (i < 7)
	{
		len = ft_strlen(built[i]);
		if (len < ft_strlen(compare))
			len = ft_strlen(compare);
		if (ft_strncmp(compare, built[i], len) == 0)
			cmd->type = BUILTIN;
		i++;
	}
}

void	build_handler(t_pipedata *p, t_cmd **cmds, int i)
{
	while (cmds[i])
	{
		if (cmds[i]->type == BUILTIN)
		{
			if (ft_strncmp("echo", cmds[i]->str, 4) == 0)
				echo(cmds, i + 1);
			if (ft_strncmp("exit", cmds[i]->str, 4) == 0)
				exit_arg_checker(join_full(cmds, i), p);
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

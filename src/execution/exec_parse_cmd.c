/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parse_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:51:52 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/18 01:19:31 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_cmd_args(char *cmd, char *path)
{
	char	**args;
	char	**cmd_args;
	int		i;

	args = mini_split(cmd, ' ');
	i = 0;
	while (args[i])
		i++;
	cmd_args = arena_malloc((i + 1) * sizeof(char *));
	cmd_args[0] = mini_strdup(path);
	i = 1;
	while (args[i])
	{
		cmd_args[i] = mini_strdup(args[i]);
		i++;
	}
	cmd_args[i] = NULL;
	return (cmd_args);
}

char	*check_dir(t_pipedata *p, char *cmd)
{
	t_stat	st;

	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_fprintf(2, "%s: Is a directory\n", cmd);
		ft_exit_child(p, NULL, 126);
	}
	return (mini_join(get_pwd(), cmd + 1));
}

int	safe_execve(t_pipedata *p, char *path, char **argv, char **env)
{
	t_stat	st;

	if (fstat(STDOUT_FILENO, &st) == -1 && errno == EBADF)
		ft_exit_child(p, NULL, 1);
	return (execve(path, argv, env));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:06:27 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/23 16:06:28 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process(t_vector *vector, int *pipefd, int index)
{
	t_cmd	**cmds;
	t_data	*data;
	char	*path;
	char	**cmd;
	int		infile;
	int		outfile;

	data = get_data();
	cmds = (t_cmd **)vector->data;
	if (index == 0)
	{
		infile = open(cmds[?]->str, O_RDONLY) < 0)
		if (dup2(infile, STDIN_FILENO);
		exit(1);
		if (dup2(pipefd[WRITE], STDOUT_FILENO) < 0)
		exit(1);
	}
	else
	{
		outfile = open(cmds[?]->str, O_RDONLY);
		if (dup2(pipefd[READ], STDIN_FILENO) < 0)
			exit(1);
		if (dup2(outfile, STDOUT_FILENO) < 0)
			exit(1);
	}
	if (close(pipefd[READ]) < 0)
		exit(1);
	if (close(pipefd[WRITE]) < 0)
		exit(1);
	path = get_bin_path(cmds[?]->str, data->envv);
	cmd = get_cmd_args(cmds[?]->str, path);
	if (execve(path, cmd, data->envv) < 0)
		exit(1);
}

void	minipipe(t_vector *vector)
{
	int	pipefd[2];
	int	pid[2];
	int	status;
	int	i;

	if (pipe(pipefd) < 0)
		exit(1);
	i = 0;
	while (i < 2)
	{
		pid[i] = fork();
		if (pid[i] == 0)
			child_process(vector, pipefd, i);
		i++;
	}
	i = 0;
	while (i < 2)
	{
		if (close(pipefd[READ]) < 0)
			exit(1);
		if (close(pipefd[WRITE]) < 0)
			exit(1);
		if (waitpid(pid[i], &status, 0) < 0)
			exit(1);
		i++;
	}
}

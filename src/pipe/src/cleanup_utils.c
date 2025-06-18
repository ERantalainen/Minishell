/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 13:18:22 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/17 18:34:41 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	free_struct(t_pipex *p)
{
	int	i;

	if (p)
	{
		if (p->infile >= 0)
			close(p->infile);
		if (p->outfile >= 0)
			close(p->outfile);
		if (p->path)
			free(p->path);
		if (p->pid)
			free(p->pid);
		free_arrays(p->cmd_args, NULL);
		i = 0;
		if (p->pipefd)
		{
			while (i < p->pipe_count && p->pipefd[i])
				free(p->pipefd[i++]);
			free(p->pipefd);
		}
		if (p->is_heredoc == true)
			if (unlink("./heredoc_.txt") == -1)
				ft_printf(STDERR_FILENO, "unlink failed\n");
		free(p);
	}
}

void	exit_handler(int code, char *param1, void *param2)
{
	t_pipex	*p;

	p = (t_pipex *)param2;
	free_struct(p);
	if (param1)
	{
		if (code == 21 && ft_strchr(param1, '/'))
			ft_printf(STDERR_FILENO, "%s: Is a directory\n", param1);
		else if (code == 127)
			ft_printf(STDERR_FILENO, "%s: command not found\n", param1);
		else
			ft_printf(STDERR_FILENO, "%s\n", param1);
	}
	exit(code);
}

void	free_arrays(char **array1, char **array2)
{
	int	i;

	if (array1)
	{
		i = 0;
		while (array1[i])
			free(array1[i++]);
		free(array1);
	}
	if (array2)
	{
		i = 0;
		while (array2[i])
			free(array2[i++]);
		free(array2);
	}
}

void	free_exit(t_pipex *p, char **array1, char **array2, bool status)
{
	int	i;

	if (array1)
	{
		i = 0;
		while (array1[i])
			free(array1[i++]);
		free(array1);
	}
	if (array2)
	{
		i = 0;
		while (array2[i])
			free(array2[i++]);
		free(array2);
	}
	if (status == true)
	{
		if (!p->path)
			exit_handler(errno, "command not found:", p);
		exit_handler(errno, NULL, p);
	}
	return ;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utility_extra.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:59:20 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/12 23:53:51 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	path_exists(void)
{
	char	*path;

	path = find_export("PATH");
	if (ft_strcmp(path, "") == 0)
		return (-1);
	return (1);
}

void	safe_close(int fd)
{
	if (close(fd) < 0)
		perror("close");
}

int	safe_dup(int fd)
{
	int	new_fd;

	new_fd = dup(fd);
	if (new_fd < 0)
	{
		perror("dup");
		return (-1);
	}
	return (new_fd);
}

int	init_pipedata(t_pipedata *p)
{
	p->index = 0;
	p->pipe_count = 0;
	p->cmd_index = 0;
	p->pipe_index = 0;
	p->infile = safe_dup(STDIN_FILENO);
	p->outfile = safe_dup(STDOUT_FILENO);
	p->stdin_copy = safe_dup(STDIN_FILENO);
	p->stdout_copy = safe_dup(STDOUT_FILENO);
	if (p->infile < 0 || p->outfile < 0
		|| p->stdin_copy < 0 || p->stdout_copy < 0)
	{
		if (p->infile >= 0)
			safe_close(p->infile);
		if (p->outfile >= 0)
			safe_close(p->outfile);
		if (p->stdin_copy >= 0)
			safe_close(p->stdin_copy);
		if (p->stdout_copy >= 0)
			safe_close(p->stdout_copy);
		return (-1);
	}
	return (0);
}

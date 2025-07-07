/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utility2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:19:02 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/04 17:34:18 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_handler(t_pipedata *p, const char *path)
{
	int	fd;

	fd = open(p->cmd_args[0], O_RDONLY);
	if (fd < 0)
	{
		if (errno == EISDIR)
			ft_fprintf(2, "%s: Is a directory\n", p->cmd_args[0]);
		else if (errno == ENOTDIR)
			ft_fprintf(2, "%s: Not a directory\n", p->cmd_args[0]);
		else if (errno == EACCES)
			ft_fprintf(2, "%s: Permission denied\n", p->cmd_args[0]);
		else if (errno == ENOENT && access(path, X_OK) < 0
			&& (ft_strchr(p->cmd_args[0], '/') || ft_strchr(p->cmd_args[0],
					'\\')))
		{
			ft_fprintf(2, "%s: No such file or directory\n", p->cmd_args[0]);
			exit(2);
		}
	}
}

void	open_file(t_cmd **tokens, t_pipedata *p, int settings, int file)
{
	if (file)
		close(file);
	if (tokens[p->index]->type == INPUT || tokens[p->index]->type == HERE_DOC)
	{
		file = open(tokens[p->index + 1]->str, settings);
	}
	else
	{
		file = open(tokens[p->index + 1]->str, settings, 0644);
	}
}

void	check_for_redirects(t_cmd **tokens, t_pipedata *p)
{
	while (tokens[p->index])
	{
		if (tokens[p->index]->type == PIPE)
			return ;
		if (tokens[p->index]->type == OUTPUT)
			open_file(tokens, p, OUTPUT_CONF, p->outfile);
		else if (tokens[p->index]->type == APPEND)
			open_file(tokens, p, APPEND_CONF, p->outfile);
		else if (tokens[p->index]->type == INPUT)
			open_file(tokens, p, INPUT_CONF, p->infile);
		else if (tokens[p->index]->type == HERE_DOC)
			open_file(tokens, p, INPUT_CONF, p->infile);
		p->index++;
	}
}

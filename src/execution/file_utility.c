/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utility.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:00:58 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/17 01:55:59 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_open_errno(t_pipedata *p, const char *file)
{
	if (errno == EISDIR)
		ft_fprintf(2, "%s: Is a directory\n", file);
	else if (errno == ENOTDIR)
		ft_fprintf(2, "%s: Not a directory\n", file);
	else if (errno == EACCES)
		ft_fprintf(2, "%s: Permission denied\n", file);
	else if (errno == ENOENT && access(file, X_OK) < 0 && (ft_strchr(file, '/')
			|| ft_strchr(file, '\\')))
	{
		ft_fprintf(2, "minishell: %s: No such file or directory\n", file);
		if (p->is_builtin == false && p->pipe_count != 0)
			ft_exit_child(p, NULL, 1);
		return (replace_export("?=1"), -1);
	}
	else if (errno == ENOENT && access(file, X_OK) < 0)
	{
		ft_fprintf(2, "minishell: %s: No such file or directory\n", file);
		if (p->is_builtin == false && p->pipe_count != 0)
			ft_exit_child(p, NULL, 1);
		return (replace_export("?=1"), -1);
	}
	if (p->is_builtin == false && p->pipe_count != 0)
		ft_exit_child(p, NULL, 1);
	return (replace_export("?=1"), -1);
}

int	open_file(t_cmd **tokens, t_pipedata *p, int settings)
{
	if (tokens[p->index]->type == INPUT || tokens[p->index]->type == HERE_DOC)
	{
		if (p->infile)
			safe_close(&p->infile);
		p->infile = open(tokens[p->index + 1]->str, settings);
		if (p->infile < 0)
			return (check_open_errno(p, tokens[p->index + 1]->str));
	}
	else
	{
		if (p->outfile)
			safe_close(&p->outfile);
		p->outfile = open(tokens[p->index + 1]->str, settings, 0644);
		if (p->outfile < 0)
			return (check_open_errno(p, tokens[p->index + 1]->str));
		p->has_out_redirect = true;
	}
	return (0);
}

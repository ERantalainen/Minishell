/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:19:02 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/13 23:32:40 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_open_error(t_pipedata *p, const char *cmd, const char *path)
{
	if (errno == EISDIR && ft_strchr(cmd, '\\'))
	{
		ft_fprintf(2, "%s: Is a directory\n", cmd);
		ft_exit_child(p, NULL, 126);
	}
	else if (errno == ENOTDIR)
	{
		ft_fprintf(2, "%s: Not a directory\n", cmd);
		ft_exit_child(p, NULL, 126);
	}
	else if (errno == EACCES)
	{
		ft_fprintf(2, "%s: Permission denied\n", cmd);
		ft_exit_child(p, NULL, 126);
	}
	else if (errno == ENOENT && access(path, X_OK) < 0
		&& (ft_strchr(cmd, '/')))
	{
		ft_fprintf(2, "%s: No such file or directory\n", cmd);
		ft_exit_child(p, NULL, 127);
	}
}

int	open_handler(t_pipedata *p, const char *path)
{
	t_stat	st;
	int		fd;

	if (!p->cmd_args[0])
		return (-1);
	if (ft_strcmp(p->cmd_args[0], "..") == 0 || ft_strcmp(p->cmd_args[0], ".") == 0)
		ft_exit_child(p, mini_join(p->cmd_args[0], CMD), 127);
	if (stat(p->cmd_args[0], &st) == 0 && S_ISDIR(st.st_mode))
	{
		if (ft_strchr(p->cmd_args[0], '/'))
		{
			ft_fprintf(2, "%s: Is a directory\n", p->cmd_args[0]);
			ft_exit_child(p, NULL, 126);
		}
		ft_fprintf(2, "%s: command not found\n", p->cmd_args[0]);
		ft_exit_child(p, NULL, 127);
	}
	fd = open(p->cmd_args[0], O_RDONLY);
	if (fd < 0)
		return (handle_open_error(p, p->cmd_args[0], path), -1);
	else if (fd >= 0)
		if (ft_strncmp(p->cmd_args[0], "./", 2) == 0 && access(p->cmd_args[0], X_OK) != 0)
			handle_open_error(p, p->cmd_args[0], path);
	safe_close(&fd);
	return (0);
}

void	check_open_errno(t_pipedata *p, const char *file)
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
		ft_exit_child(p, NULL, 1);
	}
	else if (errno == ENOENT && access(file, X_OK) < 0)
	{
		ft_fprintf(2, "minishell: %s: No such file or directory\n", file);
		ft_exit_child(p, NULL, 1);
	}
	ft_exit_child(p, NULL, 1);
}

void	open_file(t_cmd **tokens, t_pipedata *p, int settings)
{
	if (tokens[p->index]->type == INPUT || tokens[p->index]->type == HERE_DOC)
	{
		if (p->infile)
			safe_close(&p->infile);
		p->infile = open(tokens[p->index + 1]->str, settings);
		if (p->infile < 0)
			check_open_errno(p, tokens[p->index + 1]->str);
	}
	else
	{
		if (p->outfile)
			safe_close(&p->outfile);
		p->outfile = open(tokens[p->index + 1]->str, settings, 0644);
		if (p->outfile < 0)
			check_open_errno(p, tokens[p->index + 1]->str);
		p->has_out_redirect = true;
	}
}

void	check_for_redirects(t_cmd **tokens, t_pipedata *p)
{
	while (tokens[p->index])
	{
		if (tokens[p->index]->type == PIPE)
			return ;
		if (tokens[p->index]->type == OUTPUT && tokens[p->index]->next != EMPTY)
			open_file(tokens, p, OUTPUT_CONF);
		else if (tokens[p->index]->type == APPEND
			&& tokens[p->index]->next != EMPTY)
			open_file(tokens, p, APPEND_CONF);
		else if (tokens[p->index]->type == INPUT
			&& tokens[p->index]->next != EMPTY)
			open_file(tokens, p, INPUT_CONF);
		else if (tokens[p->index]->type == HERE_DOC)
			open_file(tokens, p, INPUT_CONF);
		p->index++;
	}
}

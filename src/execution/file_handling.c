/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:19:02 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/11 22:36:53 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_open_error(const char *cmd, const char *path)
{
	if (errno == EISDIR && ft_strchr(cmd, '\\'))
		ft_fprintf(2, "%s: Is a directory\n", cmd);
	else if (errno == ENOTDIR)
		ft_fprintf(2, "%s: Not a directory\n", cmd);
	else if (errno == EACCES)
		ft_fprintf(2, "%s: Permission denied\n", cmd);
	else if (errno == ENOENT && access(path, X_OK) < 0
		&& (ft_strchr(cmd, '/')))
	{
		ft_fprintf(2, "%s: No such file or directory\n", cmd);
		ft_exit_child(NULL, 127);
	}
}

void	open_handler(t_pipedata *p, const char *path)
{
	t_stat	st;
	int		fd;
	char	*cmd;

	cmd = p->cmd_args[0];
	if (!cmd)
		return ;
	if (ft_strcmp(cmd, "..") == 0 || ft_strcmp(cmd, ".") == 0)
		ft_exit_child(mini_join(cmd, CMD), 127);
	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
	{
		if (ft_strchr(cmd, '/'))
		{
			ft_fprintf(2, "%s: Is a directory\n", cmd);
			ft_exit_child(NULL, 126);
		}
		ft_fprintf(2, "%s: command not found\n", cmd);
		ft_exit_child(NULL, 127);
	}
	fd = open(cmd, O_RDONLY);
	if (fd < 0)
		handle_open_error(cmd, path);
	else
		close(fd);
}

void	check_open_errno(const char *file)
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
		ft_exit_child(NULL, 1);
	}
	else if (errno == ENOENT && access(file, X_OK) < 0)
	{
		ft_fprintf(2, "minishell: %s: No such file or directory\n", file);
		ft_exit_child(NULL, 1);
	}
	ft_exit_child(NULL, 1);
}

void	open_file(t_cmd **tokens, t_pipedata *p, int settings)
{
	if (tokens[p->index]->type == INPUT || tokens[p->index]->type == HERE_DOC)
	{
		if (p->infile)
			close(p->infile);
		p->infile = open(tokens[p->index + 1]->str, settings);
		if (p->infile < 0)
			check_open_errno(tokens[p->index + 1]->str);
	}
	else
	{
		if (p->outfile)
			close(p->outfile);
		p->outfile = open(tokens[p->index + 1]->str, settings, 0644);
		if (p->outfile < 0)
			check_open_errno(tokens[p->index + 1]->str);
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

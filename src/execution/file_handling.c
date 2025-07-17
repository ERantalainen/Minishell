/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 21:19:02 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/18 01:20:55 by jpelline         ###   ########.fr       */
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

static void	error_checker(t_pipedata *p)
{
	if (ft_strchr(p->cmd_args[0], '/'))
	{
		ft_fprintf(2, "%s: Is a directory\n", p->cmd_args[0]);
		ft_exit_child(p, NULL, 126);
	}
	ft_fprintf(2, "%s: command not found\n", p->cmd_args[0]);
	ft_exit_child(p, NULL, 127);
}

int	open_handler(t_pipedata *p, const char *path)
{
	t_stat	st;
	int		fd;

	if (!p->cmd_args[0])
		return (-1);
	if (ft_strcmp(p->cmd_args[0], "..") == 0
		|| ft_strcmp(p->cmd_args[0], ".") == 0)
		ft_exit_child(p, mini_join(p->cmd_args[0], CMD), 127);
	if (stat(p->cmd_args[0], &st) == 0 && S_ISDIR(st.st_mode))
		error_checker(p);
	fd = open(p->cmd_args[0], O_RDONLY);
	if (fd < 0)
		return (handle_open_error(p, p->cmd_args[0], path), -1);
	else if (fd >= 0)
		if (ft_strncmp(p->cmd_args[0], "./", 2) == 0
			&& access(p->cmd_args[0], X_OK) != 0)
			handle_open_error(p, p->cmd_args[0], path);
	safe_close(&fd);
	return (0);
}

int	check_for_redirects(t_cmd **tokens, t_pipedata *p)
{
	int	check;

	check = 0;
	while (tokens[p->index])
	{
		if (tokens[p->index]->type == PIPE)
			return (0);
		if (tokens[p->index]->type == OUTPUT)
			check = open_file(tokens, p, OUTPUT_CONF);
		else if (tokens[p->index]->type == APPEND)
			check = open_file(tokens, p, APPEND_CONF);
		else if (tokens[p->index]->type == INPUT)
			check = open_file(tokens, p, INPUT_CONF);
		else if (tokens[p->index]->type == HERE_DOC
			&& tokens[p->index + 1]->next != OUTPUT)
			check = open_file(tokens, p, INPUT_CONF);
		if (check == -1)
			return (-1);
		p->index++;
	}
	return (check);
}

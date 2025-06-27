/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:29:23 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/27 15:32:46 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_handler_vector(t_vector *fd_vector)
{
	size_t	i;

	if (fd_vector->data[0])
	{
		i = 0;
		while (i < fd_vector->count)
		{
			close(*(int *)fd_vector->data[i]);
			i++;
		}
	}
}

// create vector for fds
t_vector	*check_redirects(t_cmd **cmd)
{
	int			i;
	t_vector	*fd_vector;
	int			*fd;

	i = 0;
	fd_vector = new_vector(1);
	fd = arena_malloc(sizeof(int));
	while (cmd[i])
	{
		if (cmd[i]->type == OUTPUT && cmd[i]->next != EMPTY)
		{
			*fd = open(cmd[i + 1]->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			add_elem(fd_vector, fd);
			fd = arena_malloc(sizeof(int));
		}
		if (cmd[i]->type == APPEND)
		{
			*fd = open(cmd[i + 1]->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
			add_elem(fd_vector, fd);
			fd = arena_malloc(sizeof(int));
		}
		i++;
	}
	return (fd_vector);
}

// Exec a single cmd
void	exec_single_cmd(t_cmd **cmd, char **env)
{
	int			status;
	char		**cmd_args;
	int			pid;
	char		**ptr;
	char		*path;
	int			size;
	int			j;
	size_t		i;
	int			fd;
	int			stdin_copy;
	t_vector	*fd_vector;
	int			*output_fd;
	int			stdout_copy;

	stdin_copy = 0;
	stdout_copy = 0;
	stdin_copy = dup(STDIN_FILENO);
	cmd_args = mini_split(cmd[0]->str, ' ');
	size = 0;
	while (cmd[size]->next != EMPTY)
		size++;
	i = 0;
	while (cmd_args[i++] != NULL)
		size++;
	ptr = arena_malloc((size + 1) * sizeof(char *));
	j = 0;
	while (cmd_args[j] != NULL)
	{
		ptr[j] = mini_strdup(cmd_args[j]);
		j++;
	}
	i = 0;
	while (cmd[i]->next != EMPTY)
	{
		if (cmd[i + 1]->type == INPUT)
		{
			fd = open(cmd[i + 1]->str, O_RDONLY);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		if (cmd[i + 1]->type != STRING && cmd[i + 1]->type != FILES)
			break ;
		ptr[j] = mini_strdup(cmd[i + 1]->str);
		j++;
		i++;
	}
	ptr[j] = NULL;
	fd_vector = check_redirects(cmd);
	if (fd_vector->data[0])
	{
		output_fd = fd_vector->data[fd_vector->count - 1];
		stdout_copy = dup(STDOUT_FILENO);
		dup2(*output_fd, STDOUT_FILENO);
		close(*output_fd);
	}
	if (cmd[0]->type == BUILTIN)
	{
		build_handler(cmd);
		close_handler_vector(fd_vector);
		dup2(stdout_copy, STDOUT_FILENO);
		dup2(stdin_copy, STDIN_FILENO);
		return ;
	}
	reset_sig();
	pid = fork();
	if (pid == 0)
	{
		if (access(cmd_args[0], X_OK) >= 0)
			if (execve(cmd_args[0], ptr, env) < 0)
				exit(1);
		path = get_bin_path(cmd_args[0], env);
		if (execve(path, ptr, env) < 0)
			exit(1);
	}
	ignore();
	close_handler_vector(fd_vector);
	dup2(stdout_copy, STDOUT_FILENO);
	dup2(stdin_copy, STDIN_FILENO);
	waitpid(pid, &status, 0);
	child_died(status);
	catcher();
}

// exec with input <
void	exec_input(t_cmd **cmd, char **env)
{
	int			status;
	int			pid;
	int			fd;
	char		**cmd_args;
	char		*path;
	char		**ptr;
	int			size;
	size_t		i;
	int			j;
	t_vector	*fd_vector;
	int			*output_fd;
	int			stdout_copy;
	int			stdin_copy;

	stdout_copy = 0;
	stdin_copy = 0;
	stdin_copy = dup(STDIN_FILENO);
	fd = open(cmd[1]->str, O_RDONLY);
	if (fd < 0 || cmd[1]->next == EMPTY)
		return ;
	cmd_args = mini_split(cmd[2]->str, ' ');
	if (dup2(fd, STDIN_FILENO) < 0)
		return ;
	size = 0;
	while (cmd[size + 2]->next != EMPTY)
		size++;
	i = 0;
	while (cmd_args[i++] != NULL)
		size++;
	ptr = arena_malloc((size + 2) * sizeof(char *));
	j = 0;
	while (cmd_args[j] != NULL)
	{
		ptr[j] = mini_strdup(cmd_args[j]);
		j++;
	}
	i = 0;
	while (cmd[i + 2]->next != EMPTY)
	{
		if (cmd[i + 3]->type != STRING || cmd[i + 3]->type != FILES)
			break ;
		ptr[j] = mini_strdup(cmd[i + 3]->str);
		i++;
		j++;
	}
	ptr[j] = NULL;
	fd_vector = check_redirects(cmd);
	if (fd_vector->data[0])
	{
		output_fd = fd_vector->data[fd_vector->count - 1];
		stdout_copy = dup(STDOUT_FILENO);
		dup2(*output_fd, STDOUT_FILENO);
		close(*output_fd);
	}
	if (cmd[0]->type == BUILTIN)
	{
		build_handler(cmd);
		close_handler_vector(fd_vector);
		close(fd);
		dup2(stdout_copy, STDOUT_FILENO);
		dup2(stdin_copy, STDIN_FILENO);
		return ;
	}
	reset_sig();
	pid = fork();
	if (pid == 0)
	{
		if (access(cmd_args[0], X_OK) >= 0)
			if (execve(cmd_args[0], ptr, env) < 0)
				exit(1);
		path = get_bin_path(cmd_args[0], env);
		if (execve(path, ptr, env) < 0)
			exit(1);
	}
	ignore();
	close(fd);
	close_handler_vector(fd_vector);
	dup2(stdout_copy, STDOUT_FILENO);
	dup2(stdin_copy, STDIN_FILENO);
	waitpid(pid, &status, 0);
	child_died(status);
	catcher();
}

// Without pipes
void	normal_exec(t_cmd **cmd, char **env)
{
	if (cmd[0]->type == INPUT || cmd[0]->type == OUTPUT)
	{
		if (cmd[0]->next == EMPTY)
			return ;
		exec_input(cmd, env);
	}
	else
		exec_single_cmd(cmd, env);
}

// Check whether to execute with pipes or no
void	execution(t_vector *tokens, char **env)
{
	t_cmd	**cmd;
	int		i;

	i = 0;
	cmd = (t_cmd **)tokens->data;
	while (cmd[i])
	{
		if (cmd[i]->type == PIPE && cmd[i]->next != EMPTY)
		{
			setup_pipeline(cmd, env);
			return ;
		}
		i++;
	}
	normal_exec(cmd, env);
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_vector	*commands;
	t_data		*data;

	data = get_data();
	init_data(env);
	catcher();
	increase_shell_lvl();
	if (ac > 1)
		non_interactive(av, ac);
	else
	{
		while (1)
		{
			if (1)
			{
				data->valid = 1;
				input = take_input();
				add_history(input);
				commands = create_commands(token_vector(input));
				// for (size_t i = 0; i < commands->count; i++)
				// {
				// 	t_cmd *cmd = commands->data[i];
				// 	printf("%zu %s %d\n", i, cmd->str, cmd->type);
				// }
				if (data->valid == 1)
					execution(commands, vec_to_array(data->env_vec));
				free(input);
				data->line++;
			}
		}
	}
}

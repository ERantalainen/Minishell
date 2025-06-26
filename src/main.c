/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:29:23 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/26 18:13:42 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Exec with pipes
void	exec_with_pipes(t_cmd **cmd, char **env)
{
	(void)cmd;
	(void)env;
	puts("pipe");
}

// Exec a single cmd
void	exec_single_cmd(t_cmd **cmd, char **env)
{
	int		status;
	char	**cmd_args;
	int		pid;
	char	**ptr;
	char	*path;
	int		size;
	int		j;
	int		i;

	cmd_args = ft_split(cmd[0]->str, ' ');
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
		ptr[j++] = ft_strdup(cmd[i++ + 1]->str);
	ptr[j] = NULL;
	if (access(cmd_args[0], X_OK) >= 0)
	{
		reset_sig();
		pid = fork();
		if (pid == 0)
		{
			if (execve(cmd_args[0], ptr, env) < 0)
				exit(1);
		}
		else if (waitpid(pid, &status, 0) < 0)
			exit(1);
	}
	else
	{
		reset_sig();
		pid = fork();
		if (pid == 0)
		{
			path = get_bin_path(cmd_args[0], env);
			execve(path, ptr, env);
			exit(1);
		}
		else
		{
			ignore();
			if (waitpid(pid, &status, 0) < 0)
				exit(1);
			catcher();
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
	fd_vector = new_vector(5);
	fd = arena_malloc(sizeof(int));
	while (cmd[i])
	{
		if (cmd[i]->type == OUTPUT)
		{
			*fd = open(cmd[i + 1]->str, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			add_elem(fd_vector, fd);
		}
		if (cmd[i]->type == APPEND)
		{
			*fd = open(cmd[i + 1]->str, O_WRONLY | O_APPEND | O_CREAT, 0644);
			add_elem(fd_vector, fd);
		}
		i++;
	}
	return (fd_vector);
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
	int			i;
	int			j;
	t_vector	*fd_vector;
	int			*output_fd;

	reset_sig();
	pid = fork();
	if (pid == 0)
	{
		cmd_args = ft_split(cmd[2]->str, ' ');
		fd = open(cmd[1]->str, O_RDONLY);
		if (fd < 0)
			exit(1);
		if (dup2(fd, STDIN_FILENO) < 0)
			exit(1);
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
			ptr[j++ + 1] = mini_strdup(cmd[i++ + 3]->str);
		ptr[j + 1] = NULL;
		fd_vector = check_redirects(cmd);
		if (fd_vector->data[0])
		{
			output_fd = fd_vector->data[fd_vector->count - 1];
			dup2(*output_fd, STDOUT_FILENO);
		}
		if (access(cmd_args[0], X_OK) >= 0)
		{
			if (execve(cmd_args[0], ptr, env) < 0)
				exit(1);
		}
		else
		{
			path = get_bin_path(cmd_args[0], env);
			if (execve(path, ptr, env) < 0)
				exit(1);
		}
	}
	ignore();
	if (waitpid(pid, &status, 0) < 0)
		exit(1);
	catcher();
}

void	exec_output(t_cmd **cmd, char **env)
{
	(void)cmd;
	(void)env;
}

// Without pipes
void	normal_exec(t_cmd **cmd, char **env)
{
	if (cmd[0]->type == INPUT)
		exec_input(cmd, env);
	else if (cmd[0]->type == OUTPUT)
		exec_output(cmd, env);
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
	while (cmd[i]->next != EMPTY)
	{
		if (cmd[i++]->type == PIPE)
		{
			exec_with_pipes(cmd, env);
			return ;
		}
	}
	normal_exec(cmd, env);
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_vector	*commands;
	t_cmd		*test;
	t_data		*data;

	data = get_data();
	init_data(env);
	catcher();
	(void)ac;
	(void)av;
	increase_shell_lvl();
	while (1)
	{
		input = take_input();
		add_history(input);
		if (*input)
		{
			commands = create_commands(token_vector(input));
			for (size_t i = 0; i < commands->count; i++)
			{
				test = commands->data[i];
				printf("%s, %d, %d\n", test->str, test->type, test->next);
			}
			build_handler(commands);
			execution(commands, vec_to_array(data->env_vec));
		}
		free(input);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:29:23 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/24 14:33:36 by jpelline         ###   ########.fr       */
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

// Exec singular cmd, duh
void	exec_single_cmd(t_cmd *cmd, char **env)
{
	int		status;
	char	**cmd_args;
	int		pid;
	char	*path;

	cmd_args = ft_split(cmd->str, ' ');
	if (access(cmd_args[0], X_OK) >= 0)
	{
		pid = fork();
		if (pid == 0)
		{
			execve(cmd_args[0], cmd_args, env);
			exit(1);
		}
		else
		{
			if (waitpid(pid, &status, 0) < 0)
				exit(1);
		}
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			path = get_bin_path(cmd_args[0], env);
			execve(path, cmd_args, env);
			exit(1);
		}
		else
		{
			if (waitpid(pid, &status, 0) < 0)
				exit(1);
		}
	}
}

void	exec_input(t_cmd **cmd, char **env)
{
	int		status;
	int		pid;
	int		fd;
	char	**cmd_args;
	char	*path;
	char	**ptr;

	pid = fork();
	if (pid == 0)
	{
		cmd_args = ft_split(cmd[2]->str, ' ');
		fd = open(cmd[1]->str, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		path = get_bin_path(cmd_args[0], env);
		if (ft_strcmp(cmd_args[0], "grep") == 0)
		{
			ptr = cmd_args;
			ptr[1] = cmd[3]->str;
			ptr[2] = NULL;
			int pid2 = fork();
			if (pid2 == 0)
			{
				int i = 1;
				while (cmd[i + 1]->next != EMPTY)
				{
					ptr[2] = cmd[i + 3]->str;
					ptr[3] = NULL;
					int pid3 = fork();
					if (pid3 == 0)
					{
						if (execve(path, ptr, env) < 0)
							perror("error");
					}
					else
						waitpid(pid3, &status, 0);
					i++;
				}
				exit(1);
			}
			else
			{	
				waitpid(pid2, &status, 0);
				if (cmd[3]->next == EMPTY)
					execve(path, ptr, env);
				exit(1);
			}
		}
		execve(path, cmd_args, env);
		exit(1);
	}
	else
	{
		if (waitpid(pid, &status, 0) < 0)
			exit(1);
	}
}

void	exec_output(t_cmd **cmd, char **env)
{
	(void)cmd;
	(void)env;
}

// Without pipes
void	normal_exec(t_cmd **cmd, char **env)
{
	if (cmd[0]->type == STRING && cmd[0]->next == EMPTY)
	{
		exec_single_cmd(cmd[0], env);
		return ;
	}
	if (cmd[0]->type == INPUT)
		exec_input(cmd, env);
	else if (cmd[0]->type == OUTPUT)
		exec_output(cmd, env);
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

	(void)ac;
	(void)av;
	while (1)
	{
		input = take_input();
		add_history(input);
		if (*input)
		{
			commands = create_commands(token_vector(input));
			execution(commands, env);
		}
		free(input);
	}
}

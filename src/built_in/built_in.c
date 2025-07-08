/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:56:30 by erantala          #+#    #+#             */
/*   Updated: 2025/07/08 16:30:29 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	built_in(t_cmd *cmd)
{
	size_t		i;
	const char	*built[8] = {"echo", "cd", "pwd", "export", "unset", "env",
			"exit", NULL};

	i = 0;
	if (cmd->type != STRING && cmd->type != FILES)
		return ;
	while (i < 7)
	{
		if (ft_strncmp(cmd->str, built[i], ft_strlen(cmd->str)) == 0)
			cmd->type = BUILTIN;
		i++;
	}
}

static void	exit_arg_checker(char *str)
{
	size_t	i;

	i = 4;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] && ft_isalpha(str[i]))
	{
		ft_fprintf(2, mini_join(mini_join(mini_join(MS, "exit: "), str), NMARG),
			mini_strndup(str, word_len(str + i, 0)));
		return ;
	}
	if (str[i] && ft_isdigit(str[i]))
	{
		while (str[i] && !ft_isspace(str[i]))
			i++;
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (str[i])
		{
			ft_fprintf(2, "minishell: exit: too many arguments\n");
			return ;
		}
	}
	ft_exit("exit", exit_calci(str));
}

void	build_handler(t_cmd **cmds)
{
	size_t	i;

	i = 0;
	while (cmds[i])
	{
		if (cmds[i]->type == BUILTIN)
		{
			if (ft_strncmp("echo", cmds[i]->str, 4) == 0)
				echo(cmds, i);
			if (ft_strncmp("exit", cmds[i]->str, 4) == 0)
				exit_arg_checker(cmds[i]->str);
			if (ft_strncmp("cd", cmds[i]->str, 2) == 0)
				cd(cmds, i);
			if (ft_strncmp("pwd", cmds[i]->str, 3) == 0)
				pwd();
			if (ft_strncmp("export", cmds[i]->str, 6) == 0)
				make_export(cmds[i]->str + 7);
			if (ft_strncmp("unset", cmds[i]->str, 5) == 0)
				unset(cmds[i]->str + 6);
			if (ft_strncmp("env", cmds[i]->str, 3) == 0)
				env();
		}
		i++;
	}
}

void	child_builds(t_cmd **cmds, char **envi, int i)
{
	while (cmds[i])
	{
		if (cmds[i]->type == BUILTIN)
		{
			if (ft_strncmp("echo", cmds[i]->str, 4) == 0)
				echo(cmds, i);
			if (ft_strncmp("exit", cmds[i]->str, 4) == 0)
				exit_arg_checker(cmds[i]->str);
			if (ft_strncmp("cd", cmds[i]->str, 2) == 0)
				cd(cmds, i);
			if (ft_strncmp("pwd", cmds[i]->str, 3) == 0)
				pwd();
			if (ft_strncmp("export", cmds[i]->str, 6) == 0)
				envi = export_to_arr(cmds[i]->str + 7, envi);
			if (ft_strncmp("unset", cmds[i]->str, 5) == 0)
				envi = unset_child(cmds[i]->str + 6, envi);
			if (ft_strncmp("env", cmds[i]->str, 3) == 0)
				env();
			return ;
		}
		i++;
	}
}

int	mini_atoi(const char *nptr)
{
	long	result;
	int		neg;

	result = 0;
	neg = 1;
	while (*nptr == ' ' || (*nptr >= '\a' && *nptr <= '\r'))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			neg = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + (*nptr - '0');
		if (result > INT_MAX)
			break ;
		nptr++;
	}
	if (result > INT_MAX)
		return (-(result - INT_MAX));
	return ((int)(result *= neg));
}

int	exit_calci(char *cmd)
{
	int	res;

	cmd += 5;
	res = atoi(cmd);
	if (res < 0)
		res = -res;
	if (res > 255)
		return (res % 255);
	else
		return (res);
}

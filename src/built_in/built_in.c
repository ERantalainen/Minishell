/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:56:30 by erantala          #+#    #+#             */
/*   Updated: 2025/07/10 16:14:06 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	built_in(t_cmd *cmd)
{
	size_t		i;
	const char	*built[8] = {"echo", "cd", "pwd", "export", "unset", "env",
			"exit", NULL};
	size_t		len;

	i = 0;
	if (cmd->type != STRING && cmd->type != FILES)
		return ;
	while (i < 7)
	{
		len = ft_strlen(built[i]);
		if (len < ft_strlen(cmd->str))
			len = ft_strlen(cmd->str);
		if (ft_strncmp(cmd->str, built[i], len) == 0)
			cmd->type = BUILTIN;
		i++;
	}
}

static void	exit_arg_checker(char *str)
{
	size_t	i;
	int		neg;

	neg = 1;
	if (!str)
		return ;
	i = 4;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (ft_atoib(str + i) && ft_atoi(str + i) == 0)
		ft_exit("exit", 0);
	str += i;
	i = 0;
	while (str[i])
	{
		if ((str[i] && (!ft_isdigit(str[i]) && !ft_isspace(str[i])) && (i == 0 && str[i] != '-')) || !str[i])
		{
			ft_fprintf(2, mini_join(mini_join(mini_join(MS, "exit: "),
			mini_strndup(str, word_len(str, 0))), NMARG));
			ft_exit("exit", 2);
		}
		else if (ft_isdigit(str[i]) && (!ft_atol(mini_strndup(str + i, word_len(str + i, 0))) && i == 0))
		{
			str += i;
			ft_fprintf(2, mini_join(mini_join(mini_join(MS, "exit: "),
			mini_strndup(str, word_len(str, 0))), NMARG));
			ft_exit("exit", 2);
		}
		if (ft_isspace(str[i]) && str[i + 1] && !ft_isspace(str[i + 1]))
		{
			ft_fprintf(2, "minishell: exit: too many arguments\n");
			replace_export("?=2");
			return ;
		}
		i++;
	}
	replace_export("?=0");
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
				echo(cmds, i + 1);
			if (ft_strncmp("exit", cmds[i]->str, 4) == 0)
				exit_arg_checker(exit_join(cmds, i));
			if (ft_strncmp("cd", cmds[i]->str, 2) == 0)
				cd(cmds, i);
			if (ft_strncmp("pwd", cmds[i]->str, 3) == 0)
				pwd();
			if (ft_strncmp("export", cmds[i]->str, 6) == 0)
				count_export(cmds, i);
			if (ft_strncmp("unset", cmds[i]->str, 5) == 0)
				count_unset(cmds, i + 1);
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
				echo(cmds, i + 1);
			if (ft_strncmp("exit", cmds[i]->str, 4) == 0)
				exit_arg_checker(exit_join(cmds, i));
			if (ft_strncmp("cd", cmds[i]->str, 2) == 0)
				cd(cmds, i);
			if (ft_strncmp("pwd", cmds[i]->str, 3) == 0)
				pwd();
			if (ft_strncmp("export", cmds[i]->str, 6) == 0)
				envi = count_export_child(cmds, i, envi);
			if (ft_strncmp("unset", cmds[i]->str, 5) == 0)
				envi = count_unset_child(cmds[i]->str + 6, envi);
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
	long long	res;

	res = ft_atol(cmd);
	return((unsigned char)res);
}

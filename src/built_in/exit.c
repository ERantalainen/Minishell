/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:29:39 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/12 12:26:20 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_for_nonnumeric(char *str, int i)
{
	if ((str[i] && (!ft_isdigit(str[i]) && !ft_isspace(str[i]))
			&& (i == 0 && str[i] != '-' && str[i] != '+')) || !str[i])
	{
		ft_fprintf(2, mini_join(mini_join(mini_join(MS, "exit: "),
					mini_strndup(str, word_len(str, 0))), NMARG));
		ft_exit("exit", 2);
	}
}

static void	overflow_error(char *str, int i)
{
	str += i;
	ft_fprintf(2, mini_join(mini_join(mini_join(MS, "exit: "),
				mini_strndup(str, word_len(str, 0))), NMARG));
	ft_exit("exit", 2);
}

static int	check_for_too_many(char *str, int i)
{
	if (ft_isspace(str[i]) && str[i + 1] && !ft_isspace(str[i + 1]))
	{
		ft_fprintf(2, "minishell: exit: too many arguments\n");
		replace_export("?=1");
		return (1);
	}
	return (0);
}

void	exit_arg_checker(char *str)
{
	size_t	i;

	if (!str)
		return ;
	i = 4;
	if (!str[i])
		ft_exit("exit", 0);
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (ft_atoib(str + i) && ft_atoi(str + i) == 0)
		ft_exit("exit", 0);
	str += i;
	if (!ft_atol(mini_strndup(str, word_len(str, 0))))
		overflow_error(str, 0);
	i = 0;
	while (str[i])
	{
		check_for_nonnumeric(str, i);
		if (check_for_too_many(str, i) == 1)
			return ;
		i++;
	}
	ft_exit("exit", exit_calci(str));
}

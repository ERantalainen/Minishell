/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:29:39 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/18 15:21:26 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	overflow_error(char *str, int i)
{
	str += i;
	ft_fprintf(2, mini_join(mini_join(mini_join(MS, "exit: "),
				mini_strndup(str, word_len(str, 0))), NMARG));
	ft_exit("exit", 2);
}

static int	check_for_too_many(void)
{
	ft_fprintf(2, "minishell: exit: too many arguments\n");
	replace_export("?=1");
	return (1);
}

static	int	count_args(char	*str)
{
	size_t	i;
	bool	alpha;

	alpha = false;
	i = 0;
	while (str[i])
	{
		if (alpha == false && ft_isalpha(str[i]))
			alpha = true;
		if (str[i] && str[i + 1]
			&& ft_isspace(str[i]) && !ft_isspace(str[i + 1]))
		{
			if (alpha == false)
				check_for_too_many();
			else
				overflow_error(str, 0);
			return (0);
		}
		i++;
	}
	return (1);
}

void	exit_arg_checker(char *str, t_pipedata *p)
{
	size_t	i;
	bool	valid;

	safe_close(&p->stdin_copy);
	safe_close(&p->stdout_copy);
	if (!str)
		return ;
	i = 5;
	if (ft_strlen(str) > 4 && ft_strcmp(str + i, "") == 0)
		overflow_error(str, i);
	if (!str[i])
		ft_exit("exit", 0);
	while (str[i] && ft_isspace(str[i]))
		i++;
	str += i;
	if (count_args(str) == 0)
		return ;
	exit_atoi(str, &valid);
	if (!valid)
		overflow_error(str, 0);
	ft_exit("exit", exit_calci(str));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:39:37 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/18 02:04:50 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_option(char *s, bool quoted, bool *nl)
{
	int	i;

	i = 0;
	if (!s || !s[i] || s[i] != '-')
		return (-1);
	else if (!s[i + 1] && s[i] == '-')
		return (-1);
	i++;
	while (s[i] && s[i] == 'n')
		i++;
	if (s[i] && s[i] != 'n' && quoted)
		return (-1);
	else if (s[i] && (s[i] != 'n' || ft_isspace(s[i])))
	{
		while (s[i] && ft_isspace(s[i]))
			i++;
		if (s[i])
			return (-1);
	}
	*nl = false;
	return (i);
}

int	echo_part(t_cmd **c, int *pos, bool *nl, bool qt)
{
	char	*command;

	qt = c[*pos]->quoted;
	if (c[*pos]->space)
	{
		command = c[*pos]->str;
		(*pos)++;
	}
	else
	{
		command = "";
		while (c[*pos] && (c[*pos]->type == FILES || c[*pos]->type == STRING))
		{
			if (c[*pos]->quoted)
				qt = 1;
			if (!c[*pos]->space)
			{
				command = mini_join(command, c[*pos]->str);
				(*pos)++;
			}
			else
				break ;
		}
	}
	return (valid_option(command, qt, nl));
}

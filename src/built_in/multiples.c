/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiples.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:02:10 by erantala          #+#    #+#             */
/*   Updated: 2025/07/11 01:31:04 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_exports(t_cmd **cmds, int *i)
{
	char	*export;
	size_t	j;

	j = 0;
	if (!(cmds[*i] && ((cmds[*i]->type == FILES || cmds[*i]->type == STRING)
	|| (cmds[*i]->type == BUILTIN))))
		return (NULL);
	while(cmds[*i]->str[j] && ft_isspace(cmds[*i]->str[j]))
		j++;
	export = mini_strdup(cmds[*i]->str + j);
	(*i)++;
	while (cmds[*i] && ((cmds[*i]->type == FILES || cmds[*i]->type == STRING)
		|| (cmds[*i]->type == BUILTIN)))
	{
		if (cmds[*i]->space == 0)
			export = mini_join(export, cmds[*i]->str);
		else
			break ;
		(*i)++;
	}
	return (export);
}

static int	check_emp(t_cmd **cmds, int i)
{
	if (cmds[i]->next != STRING && cmds[i]->next != FILES)
	{
		empty_export();
		return (0);
	}
	else
		return (1);
}

int	count_export(t_cmd **cmds, int i)
{
	char		*export;
	char		**exps;
	size_t		j;

	if (!check_emp(cmds, i))
		return (0);
	exps = arena_malloc(sizeof(char *) * 2);
	i++;
	while (1)
	{
		export = build_exports(cmds, &i);
		if (!export)
			break ;
		exps = ft_stradd(exps, export);
	}
	j = 0;
	if (check_export(exps))
	{
		while (exps[j])
		{
			make_export(exps[j]);
			j++;
		}
	}
	return (0);
}

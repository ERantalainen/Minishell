/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiples.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:02:10 by erantala          #+#    #+#             */
/*   Updated: 2025/07/09 23:51:38 by erantala         ###   ########.fr       */
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

static bool	export_str(char *s)
{

	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=' && s[i + 1] && i != 0)
			break ;
		if (!ft_isalpha(s[0]) || ((ft_isalnum(s[i]) == 0) && s[i]))
		{
			ft_fprintf(2, "minishell: export: `%s", mini_join(s, INV));
			replace_export("?=1");
			return (false);
		}
		i++;
	}
	if (s[i] != '=')
		return (false);
	return (true);
}

int	check_export(char **exports)
{
	size_t	i;

	i = 0;
	while (exports[i])
	{
		if (!export_str(exports[i]))
			return (0);
		i++;
	}
	return (1);
}

int	count_export(t_cmd **cmds, int i)
{
	char		*export;
	char		**exps;
	size_t		j;

	exps = arena_malloc(sizeof(char *) * 2);
	if (cmds[i]->next != STRING && cmds[i]->next != FILES)
	{
		empty_export();
		return (0);
	}
	else
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

char	**count_export_child(t_cmd **cmds, int i, char **envi)
{
	char		*export;

	if (cmds[i]->next != STRING && cmds[i]->next != FILES)
	{
		empty_export();
		return (envi);
	}
	else
		i++;
	while (1)
	{
		export = build_exports(cmds, &i);
		if (!export)
			break ;
		if (check_export(&export))
			envi = export_to_arr(export, envi);
	}
	return (envi);
}

char	**count_unset_child(char *command, char **env)
{
	int		len;
	size_t	i;

	while (command[0])
	{
		i = 0;
		while (command[i] && ft_isspace(command[i]))
			i++;
		if (!command[i])
			break ;
		len = word_len(command + i, 0);
		if (len > 0)
			unset_child(mini_strndup(command + i, len), env);
		else
			break ;
		command += len + i;
	}
	return (env);
}


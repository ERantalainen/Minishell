/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 03:49:53 by erantala          #+#    #+#             */
/*   Updated: 2025/07/10 22:59:18 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_stradd(char **s, char *line)
{
	size_t	i;
	char	**new;

	i = 0;
	new = arena_malloc(sizeof(char *) * (ft_stralen(s) + 2));
	while (s[i])
	{
		new[i] = mini_strdup(s[i]);
		i++;
	}
	new[i] = line;
	new[i + 1] = NULL;
	return (new);
}

char	**ft_strremove(char	**s, int i)
{
	while (s[i + 1])
	{
		s[i] = s[i + 1];
		i++;
	}
	s[i] = NULL;
	return (s);
}

char	**replace_line(char **s, char *line, size_t i)
{
	s[i] = line;
	return (s);
}

int	find_line(char	**line, char *key)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (ft_strncmp(line[i], key, key_len(key)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	**export_to_arr(char *key, char **exports)
{
	char	**new_exp;

	new_exp = ft_stradd(exports, key);
	return (new_exp);
}

char	**make_arr_export(char *exp, char **exports)
{
	size_t	i;

	i = 0;
	if (ft_strcmp(exp, "") == 0)
		empty_export();
	while (exp[i])
	{
		if (exp[i] != '=')
			break ;
		if ((ft_isalnum(exp[i]) == 0) && exp[i])
		{
			ft_fprintf(2, "minishell: export: `%s", mini_join(exp, INV));
			exit (1);
		}
		i++;
	}
	if (find_line(exports, exp) != -1)
		exports = replace_line(exports, exp, find_line(exports, exp));
	else
		exports = export_to_arr(exp, exports);
	exit (0);
}

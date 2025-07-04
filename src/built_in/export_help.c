/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 03:49:53 by erantala          #+#    #+#             */
/*   Updated: 2025/07/04 04:10:33 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_stradd(char **s, char *line)
{
	size_t i;
	char	**new;

	i = 0;
	new = arena_malloc(sizeof(char *) + (ft_stralen(s) + 2));
	while(s[i])
	{
		new[i] = mini_strdup(s[i]);
		i++;
	}
	s[i] = line;
	s[i + 1] = NULL;
	return (new);
}

char	**export_to_arr(char *key, char **exports)
{
	char	**new_exp;

	new_exp = ft_stradd(exports, key);
	return (new_exp);
}

char	**replace_line(char **s, size_t i)
{
	while (s[i] && s[i+1])
	{
		s[i] = s[i + 1];
		i++;
	}
	s[i] = NULL;
	return (s);
}

int	find_line(char	**line, char *key)
{
	size_t	i;

	i = 0;
	while(line[i])
	{
		if (ft_strncmp(line[i], key, key_len(key)) == 0)
			return (i);
		i++
	}
	return (-1);
}
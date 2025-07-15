/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 03:49:53 by erantala          #+#    #+#             */
/*   Updated: 2025/07/15 15:50:19 by erantala         ###   ########.fr       */
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

void	export_addition(char *command)
{
	char	*original;
	char	*key;

	key = mini_strndup(command, key_len(command) - 1);
	original = find_export(key);
	original = mini_join(original, command += key_len(command) + 1);
	key = mini_join(key, "=");
	original = mini_join(key, original);
	replace_export(original);
}

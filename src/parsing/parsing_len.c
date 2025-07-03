/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_len.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 05:16:37 by erantala          #+#    #+#             */
/*   Updated: 2025/07/03 05:31:49 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_all(int c)
{
	if (c == '<' || c == '|' || c == '>' || c == '\'' || c == '"')
		return (1);
	return (0);
}

size_t	neutral_len(char *s)
{
	int		i;
	char	c;

	i = 0;
	while (s[i] && ft_isspace(s[i]) == 0 && !check_all(s[i]))
		i++;
	if (i == 0 && check_all(s[i]))
	{
		if (s[i] == '|')
			return (1);
		c = s[i];
		if (!s[i + 1] || (s[i + 1] && s[i + 1] != c))
			return (1);
		else
			return (2);
	}
	return (i);
}


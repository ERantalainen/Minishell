/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:19:17 by erantala          #+#    #+#             */
/*   Updated: 2025/07/10 15:49:22 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(void)
{
	t_data	*data;
	char	*line;
	size_t	i;

	i = 0;
	data = get_data();
	while (i < data->env_vec->count)
	{
		line = data->env_vec->data[i];
		if (ft_strncmp(line, "?=", 2) != 0)
			printf("%s\n", line);
		i++;
	}
	replace_export("?=0");
}

void	env_char(char **s)
{
	char	*line;
	size_t	i;

	i = 0;
	while (s[i])
	{
		line = s[i];
		if (ft_strncmp(line, "?=", 2) != 0)
			printf("%s\n", line);
		i++;
	}
}
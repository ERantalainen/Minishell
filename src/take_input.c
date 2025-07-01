/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 02:49:43 by erantala          #+#    #+#             */
/*   Updated: 2025/07/01 15:42:17 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*take_input()
{
	char	*input;

	while (1)
	{
		input = readline("\001\033[0;36m\002minishell>\001\033[0m\002 ");
		if (!input)
		{
			ft_exit("exit", 0);
		}
		if (input[0] != 0)
		{
			if (check_quotes(input))
			{
				free(input);
				exit(2);
			}
			return (input);
		}
	}
}

int	check_quotes(char *s)
{
	int	sgl;
	int	dbl;
	int	i;

	i = 0;
	sgl = 0;
	dbl = 0;
	while (s[i])
	{
		if (s[i] == '\'')
			sgl++;
		if (s[i] == '"')
			dbl++;
		i++;
	}
	if (sgl % 2 != 0 || dbl % 2 != 0)
		return (1);
	return (0);
}

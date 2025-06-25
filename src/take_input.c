/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 02:49:43 by erantala          #+#    #+#             */
/*   Updated: 2025/06/25 16:47:59 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*take_input()
{
	char	*input;

	while (1)
	{
		input = readline("minishell>");
		if (!input)
		{
			ft_putendl_fd("exit", 1);
			exit(0);
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

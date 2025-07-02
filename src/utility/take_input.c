/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 02:49:43 by erantala          #+#    #+#             */
/*   Updated: 2025/07/02 18:47:18 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*take_input(void)
{
	char	*input;

	while (1)
	{
		input = readline(RDPROMPT);
		if (!input)
		{
			ft_exit("exit", 0);
		}
		if (input[0] != 0)
		{
			if (check_quotes(input))
			{
				free(input);
				return (NULL);
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
	if (sgl % 2 == 0 && dbl % 2 == 0)
		return (0);
	ft_fprintf(2, "Syntax error unclosed quotes\n");
	return (1);
}

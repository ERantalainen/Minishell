/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 02:49:43 by erantala          #+#    #+#             */
/*   Updated: 2025/07/05 00:55:40 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*take_input(void)
{
	char	*input;
	char	*ret;
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
		ret = mini_strdup(input);
		free(input);
		return (ret);
		}
	}
}

int	check_quotes(char *s)
{
	int		i;
	bool	syntax;
	char	quote;
	
	syntax = 1;
	i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			quote = s[i];
			i++;
			syntax = 0;
			while (s[i] && s[i] != quote)
				i++;
			if (!s[i])
				break;
			syntax = 1;
		}
		i++;
	}
	if (syntax == 0)
	{
		ft_fprintf(2, "Syntax error unclosed quotes\n");
		return (1);
	}
	return (0);
}

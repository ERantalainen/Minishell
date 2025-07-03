/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:34:01 by erantala          #+#    #+#             */
/*   Updated: 2025/07/03 04:40:40 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	option_quotes(t_data *data)
{
	char	*check;
	size_t	j;

	j = 0;
	check = ft_strnstr(data->input, "echo", ft_strlen(data->input));
	if (check)
	{
		check += 4;
		while(check[j++])
			if ((check[j] == '\'' && check[j + 1] && check[j + 1] == '\'')
			|| (check[j] == '"' && check[j + 1] && check[j + 1] == '"'))
			{
				if (check[j + 2] && check[j + 2] == '-' 
					&& check[j + 3] && check[j + 3] == 'n')
						return (1);
				else
					return (0);
			}
		return (1);
	}
	return (1);
}


static	void options(char *command, int *i, bool *nl)
{
	t_data	*data;

	data = get_data();
	if (!option_quotes(data))
		return ;
	while (command[*i] == ' ')
		i++;
	if ((command[*i] == '-' && command[*i + 1] == 'n'))
	{
		while ((command[*i] == 'n' && command[*i - 1] != ' ')
			|| command[*i] == ' ' 
			|| (command[*i] == '-' && command[*i + 1] != ' ' 
				&& command[*i + 1] != '-'))
			(*i)++;
		*nl = 0;
		if (command[*i] == ' ')
			(*i)++;
	}
}

void	echo(t_cmd **commands, int i)
{
	char	*command;
	bool	newline;

	command = commands[i]->str + 4;
	while (commands[i]->next == FILES || commands[i]->next == STRING)
		command = mini_append(command, commands[i++ + 1]->str);
	i = 0;
	newline = 1;
	options(command, &i, &newline);
	command += i;
	if (newline == 1)
		ft_putendl_fd((mini_strndup(command, ft_strlen(command))), 1);
	else
		ft_putstr_fd((mini_strndup(command, ft_strlen(command))), 1);
	replace_export("?=0");
}

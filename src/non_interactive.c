/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:53:35 by erantala          #+#    #+#             */
/*   Updated: 2025/06/27 18:03:02 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	non_interactive(char **argv, int argc)
{
	char			*input;
	char			*add;
	t_data			*data;
	t_vector		*commands;

	data = get_data();
	input = "";
	while (1)
	{
		data->valid = 1;
		while (1)
		{
			add = get_input(argv, argc);
			if (add == NULL)
				break;
			input = mini_append(input, add);
		}
		commands = create_commands(token_vector(input));
		for (size_t i = 0; i < commands->count; i++)
		{
			t_cmd *cmd = commands->data[i];
			puts(cmd->str);
		}
		execution(commands, vec_to_array(data->env_vec));
		break ;
	}
}

char	*get_input(char **argv, int argc)
{
	static int	pos = 0;

	pos++;
	if (pos < argc)
		return (argv[pos]);
	else
		return (NULL);
}
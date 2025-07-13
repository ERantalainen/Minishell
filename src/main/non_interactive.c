/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:53:35 by erantala          #+#    #+#             */
/*   Updated: 2025/07/13 03:46:33 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	non_interactive(void)
{
	char		*line;
	t_data		*data;
	t_vector	*commands;

	data = get_data();
	data->non_interactive = true;
	while (!isatty(STDIN_FILENO))
	{
		data->valid = 1;
		line = get_next_line(STDIN_FILENO);
		if (line)
		{
			commands = create_commands(token_vector(line));
			check_command_syntax(commands, data);
			if (data->valid == 1 && commands->data)
				execution((t_cmd **)commands->data,
					vec_to_array(data->env_vec));
			clean_heredoc();
			free(line);
		}
		else
			break ;
	}
	ft_exit("", ft_atoi(find_export("$?")));
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

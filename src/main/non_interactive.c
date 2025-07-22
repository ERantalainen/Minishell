/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:53:35 by erantala          #+#    #+#             */
/*   Updated: 2025/07/22 16:42:22 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	non_interactive(t_data *data)
{
	char		*line;
	t_vector	*commands;

	data->non_interactive = true;
	while (!isatty(STDIN_FILENO))
	{
		data->valid = 1;
		line = get_next_line(STDIN_FILENO);
		if (line)
		{
			commands = create_commands(token_vector(line));
			check_command_syntax(commands, data);
			if (data->valid == 1)
				check_heredoc(commands);
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

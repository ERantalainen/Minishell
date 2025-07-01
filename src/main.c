/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:29:23 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/01 19:20:09 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_and_execute(t_data *data)
{
	t_vector	*tokens;
	char		*input;

	while (true)
	{
		if (isatty(0))
		{
			data->valid = 1;
			input = take_input();
			add_history(input);
			tokens = create_commands(token_vector(input));
			if (data->valid == 1 && tokens)
			{
				execution((t_cmd **)tokens->data, vec_to_array(data->env_vec));
				clean_heredoc();
			}
			free(input);
			data->line++;
		}
	}
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	data = get_data();
	init_data(env);
	catcher();
	increase_shell_lvl();
	if (ac > 1)
		non_interactive(av, ac);
	else
		parse_and_execute(data);
}

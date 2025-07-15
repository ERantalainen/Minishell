/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:29:23 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/15 16:49:23 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_and_execute(t_data *data)
{
	t_vector	*tokens;
	char		*input;

	while (true)
	{
		data->valid = 1;
		input = take_input();
		if (input)
		{
			add_history(input);
			tokens = create_commands(token_vector(input));
			if (data->valid == 1 && tokens)
			{
				check_command_syntax(tokens, data);
				execution((t_cmd **)tokens->data,
					vec_to_array(data->env_vec));
			}
			clean_heredoc();
			data->line++;
		}
	}
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	data = get_data();
	(void)ac;
	(void)av;
	init_data(env);
	catcher();
	increase_shell_lvl();
	if (!isatty(STDIN_FILENO))
		non_interactive();
	else
		parse_and_execute(data);
}

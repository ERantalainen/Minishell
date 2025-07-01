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

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_vector	*tokens;
	t_data		*data;

	data = get_data();
	init_data(env);
	catcher();
	increase_shell_lvl();
	if (ac > 1)
		non_interactive(av, ac);
	else
	{
		while (1)
		{
			if (1)
			{
				data->valid = 1;
				input = take_input();
				add_history(input);
				tokens = create_commands(token_vector(input));
				if (data->valid == 1 && tokens)
				{
					// for(size_t i = 0; i < tokens->count; i++)
					// {
					// 	t_cmd *test = tokens->data[i];
					// 	printf("%zu, %s, Curr: %d Next: %d\n", i, test->str, test->type, test->next);
					// }
					execution((t_cmd **)tokens->data, vec_to_array(data->env_vec));
					clean_heredoc();
				}
				free(input);
				data->line++;
			}
		}
	}
}

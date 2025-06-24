/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:29:23 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/24 01:47:37 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main()
{
	char	*input;
	t_vector *commands;
	t_cmd	*test;

	export("$world", "hello there");
	// Infinite loop for shell prompt; replace with actual shell logic
	while (1)
	{

		input = take_input();
		commands = create_commands(token_vector(input));
		free(input);
		for (size_t i = 0; i < commands->count; i++)
		{
			test = commands->data[i];
			printf("%zu: %s type: %d\n", i, test->str, test->type);
		}
		// break;
	}
}

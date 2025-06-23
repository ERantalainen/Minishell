/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:29:23 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/23 17:59:22 by erantala         ###   ########.fr       */
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
		printf("%s\n", input);
		commands = create_commands(token_vector(input));
		printf("%s\n", input);
		free(input);
		printf("%s\n", "main");
		for (size_t i = 0; i < commands->count; i++)
		{
			test = commands->data[i];
			printf("%zu: %s\n", i, test->str);
		}
		// break;
	}
}

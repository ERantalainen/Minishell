/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:29:23 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/20 15:23:36 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main()
{
	char	*input;
	t_vector *commands;
	t_cmd	*test;
	// Infinite loop for shell prompt; replace with actual shell logic
	while (1)
	{
		input = take_input();
		printf("%s\n", input);
		commands = create_commands(token_vector(input));
		free(input);
		test = commands->data[0];
		printf("%s\n", test->str);
		test = commands->data[1];
		printf("%s\n", test->str);
		// break;
	}
}

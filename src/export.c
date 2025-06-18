/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:12:54 by erantala          #+#    #+#             */
/*   Updated: 2025/06/24 16:20:27 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export(char *key, char *expansion)
{
	t_vector	*vars;
	t_var		*new;

	vars = get_vars();
	new = arena_malloc(sizeof(t_var));
	new->expansion = expansion;
	new->key = key + 1;
	add_elem(vars, (void *)new);
	return ;
}

// Exports a new variable with key and expansion.

t_vector	*get_vars()
{
	static t_vector *variables = NULL;

	if (!variables)
		variables = new_vector(10);
	return (variables);
}

// Return a vector of expanded variables.

char	*find_export(char *key)
{
	t_vector *vars;
	size_t		i;
	t_var	*search;

	vars = get_vars();
	i = 0;
	if (key[0] == '$')
		key += 1;
	printf("Checking expansion for: >%s<\n", key);
	while (i < vars->count && vars->data[i] != NULL)
	{
		search = (t_var *)vars->data[i];
		if (ft_strcmp(search->key, key) == 0)
			return (search->expansion);
		i++;
	}
	return ("");
}

// void	make_export(t_vector *cmds)
// {
// 	char	*key;
// 	char	*expansion;
// 	char	*command;
// 	t_cmd	*cmd;
// }

// Finds an exported variable by searching its key.
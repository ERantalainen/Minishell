/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:12:54 by erantala          #+#    #+#             */
/*   Updated: 2025/06/19 18:16:45 by erantala         ###   ########.fr       */
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
	new->key = key;
	add_elem(vars, (void *)new);
	return ;
}

t_vector	*get_vars()
{
	static t_vector *variables;

	if (!variables)
		variables = new_vector(10);
	return (variables);
}

char	*find_export(char *key)
{
	t_vector *vars;
	int		i;
	t_var	*search;
	int		key_len;

	key_len = word_len(key);
	vars = get_vars();
	while (i < vars->count && vars->data[i] != NULL)
	{
		search = (t_var *)vars->data[i];
		if (ft_strncmp(search->key, key, key_len) == 0)
			return (search->expansion);
		i++;
	}
	return ("");
}
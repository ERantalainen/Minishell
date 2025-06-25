/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:12:54 by erantala          #+#    #+#             */
/*   Updated: 2025/06/26 00:44:50 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export(char *export)
{
	t_data		*data;

	data = get_data();
	add_elem(data->env_vec, export);
	return ;
}

// Exports a new variable with key and expansion.


// Return a vector of expanded variables.

char	*find_export(char *key)
{
	t_data	*data;
	size_t		i;
	char	*var;
	data = get_data();

	i = 0;
	if (key[0] == '$')
		key += 1;
	while (i < data->env_vec->count)
	{
		var = data->env_vec->data[i];
		if (ft_strncmp(key, var, key_len(var)) == 0)
			return (var + (key_len(var) + 1));
		i++;
	}
	return ("");
}

size_t	key_len(char *s)
{
	size_t	i;
	
	i = 0;
	while (s[i] && s[i] != '=' && !ft_isspace(s[i]))
		i++;
	return (i);
}

// Finds an exported variable by searching its key.

void	make_export(t_vector *cmds, size_t i)
{
	char	*command;
	t_cmd	*cmd;

	cmd = cmds->data[i];
	command = cmd->str + 7;
	if (ft_strcmp("", find_export(command)) != 0)
		replace_export(command);
	else
		export(command);
}

// Makes an export by builting the key and expansion.

void	replace_export(char *key)
{
	t_data	*data;
	size_t		i;
	char	*var;

	data = get_data();
	i = 0;
	while (i < data->env_vec->count)
	{
		var = data->env_vec->data[i];
		if (ft_strncmp(key, var, key_len(var)) == 0)
		{
			data->env_vec->data[i] = key;
		}
		i++;
	}
}

// Replaces an already existing export when needed.


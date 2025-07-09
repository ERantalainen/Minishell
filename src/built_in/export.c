/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:12:54 by erantala          #+#    #+#             */
/*   Updated: 2025/07/09 03:50:41 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export(char *export)
{
	t_data	*data;

	data = get_data();
	add_elem(data->env_vec, export);
	return ;
}

// Exports a new variable with key and expansion.

// Return a vector of expanded variables.

char	*find_export(char *key)
{
	t_data	*data;
	size_t	i;
	size_t	len;
	char	*var;

	data = get_data();
	if (!key)
		return ("");
	if (ft_strlen(key) == 1)
		return (mini_strdup("$"));
	i = 0;
	if (key[0] == '$')
		key += 1;
	while (i < data->env_vec->count)
	{
		var = data->env_vec->data[i];
		len = key_len(var);
		if (key_len(key) > key_len(var))
			len = key_len(key);
		if (ft_strncmp(key, var, len) == 0)
			return (var + (key_len(var) + 1));
		i++;
	}
	return ("");
}

size_t	key_len(char *s)
{
	size_t	i;

	i = 0;
	if (s[i] == '$' && s[i + 1] == '"')
		return (2);
	if (s[i] == '$')
		i++;
	while ((s[i] && s[i] != '=' && !ft_isspace(s[i]) && s[i] != '$')
		&& !check_specials(s[i], 0))
		i++;
	return (i);
}

// Finds an exported variable by searching its key.

void	make_export(char *command)
{
	size_t	i;

	i = 0;
	if (ft_strcmp(command, "") == 0)
		empty_export();
	while (command[i])
	{
		if (command[i] != '=')
			break ;
		if ((ft_isalnum(command[i]) == 0) && command[i])
		{
			ft_fprintf(2, "minishell: export: `%s", mini_join(command, INV));
			replace_export("?=1");
			return ;
		}
		i++;
	}
	if (ft_strcmp("", find_export(command)) != 0)
		replace_export(command);
	else
	{
		export(command);
		init_export();
	}
	replace_export("?=0");
}

// Makes an export by builting the key and expansion.

void	replace_export(char *key)
{
	t_data	*data;
	size_t	i;
	char	*var;

	data = get_data();
	i = 0;
	while (i < data->env_vec->count)
	{
		var = data->env_vec->data[i];
		if (ft_strncmp(key, var, key_len(var)) == 0)
		{
			data->env_vec->data[i] = key;
			return ;
		}
		i++;
	}
	export(key);
}

// Replaces an already existing export when needed.

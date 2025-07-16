/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 01:21:51 by erantala          #+#    #+#             */
/*   Updated: 2025/07/16 17:23:39 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	export_str(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=' && i != 0)
			break ;
		if ((!ft_isalpha(s[0]) && s[0] != '_') || (((ft_isalnum(s[i]) == 0))
				&& s[i] != '_' && s[i] && s[i] != '+')
			|| ((s[i] == '+' && s[i + 1] != '=')))
		{
			ft_fprintf(2, "minishell: export: `%s", mini_join(s, INV));
			replace_export("?=1");
			return (false);
		}
		i++;
	}
	if (s[i] != '=' && s[i] != 0)
		return (false);
	return (true);
}

int	check_export(char **exports)
{
	size_t	i;

	i = 0;
	while (exports[i])
	{
		if (!export_str(exports[i]))
			return (0);
		i++;
	}
	return (1);
}

bool	find_exp_bool(char *key)
{
	t_data	*data;
	size_t	i;
	size_t	len;
	char	*var;

	data = get_data();
	if (!key)
		return ("");
	if (ft_strlen(key) == 1 && key[0] == '$')
		return (mini_strdup("$"));
	i = 0;
	if (key[0] == '$')
		key += 1;
	while (i < data->env_vec->count)
	{
		var = data->env_vec->data[i];
		len = key_len(var);
		if (key_len(key) > len)
			len = key_len(key);
		if (ft_strncmp(key, var, len) == 0)
			return (true);
		i++;
	}
	return (false);
}
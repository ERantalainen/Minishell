/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 01:21:51 by erantala          #+#    #+#             */
/*   Updated: 2025/07/13 20:24:50 by erantala         ###   ########.fr       */
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
		if ((!ft_isalpha(s[0]) && s[0] != '_') || ((ft_isalnum(s[i]) == 0)
				&& s[i] != '_' && s[i]))
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

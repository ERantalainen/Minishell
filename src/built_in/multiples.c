/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiples.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:02:10 by erantala          #+#    #+#             */
/*   Updated: 2025/07/08 19:05:48 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_len(char *s)
{
	int		i;
	int		quote;
	bool	limiter;

	limiter = 0;
	quote = -1;
	i = 0;
	while(s[i])
	{
		if(s[i] == '"' || s[i] == '\'')
			quote *= -1;
		if (quote == -1 && ft_isspace(s[i]))
			break ;
		if (s[i] == '=')
			limiter = 1;
		i++;
	}
	if (!limiter)
		return (-1);
	return (i);
}

int	count_export(char	*export)
{
	int			len;
	size_t		i;

	i = 0;
	if (ft_strlen(export) == 6)
		empty_export();
	else
		export += 6;
	while (export[0])
	{
		i = 0;
		while (export[i] && ft_isspace(export[i]))
			i++;
		if (!export[i])
			return (0);
		len = export_len(export + i);
		if (len > 0)
			make_export(mini_strndup(export + i, len));
		else
			return (0);
		export += len + i;
	}
	return (0);
}

char	**count_export_child(char	*export, char **env)
{
	int			len;
	size_t		i;

	i = 0;
	if (ft_strlen(export) == 6)
		empty_export();
	else
		export += 6;
	while (export[0])
	{
		i = 0;
		while (export[i] && ft_isspace(export[i]))
			i++;
		if (!export[i])
			break ;
		len = export_len(export + i);
		if (len > 0)
			export_to_arr(mini_strndup(export +i, len), env);
		else
			break ;
		export += len + i;
	}
		return (env);
}

char	**count_unset_child(char *command, char **env)
{
	int		len;
	size_t	i;

	while (command[0])
	{
		i = 0;
		while (command[i] && ft_isspace(command[i]))
			i++;
		if (!command[i])
			break ;
		len = word_len(command + i, 0);
		if (len > 0)
			unset_child(mini_strndup(command + i, len), env);
		else
			break ;
		command += len + i;
	}
	return (env);
}
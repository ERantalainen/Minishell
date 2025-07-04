/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:04:47 by erantala          #+#    #+#             */
/*   Updated: 2025/07/04 18:48:59 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ambigous(char *s, size_t *i)
{
	size_t	pos;
	size_t	count;
	t_data	*data;
	size_t	len;
	char	*exp_check;

	len = word_len(s + *i, '|');
	exp_check = find_export(mini_strndup(s + *i, key_len(s + *i + 1)));
	puts("here");
	if (ft_strcmp("", exp_check) != 0)
		return (NULL);
	count = 0;
	puts("here");
	pos = *i;
	while (pos < len)
	{
		if (s[pos] == '"' || s[pos] == '\'')
			return (NULL);
	}
	puts("here");
	exp_check = mini_strndup(s + *i, word_len(s + *i, 0));
	data = get_data();
	data->valid = 0;
	ft_fprintf(2, "%s %s\n", mini_join(MS, exp_check), AMBG);
	return ("");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:04:47 by erantala          #+#    #+#             */
/*   Updated: 2025/07/04 19:54:20 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_all_redir(char *s, size_t	len);
char	*ambigous(char *s, size_t i)
{
	size_t	pos;
	size_t	count;
	t_data	*data;
	size_t	len;
	char	*exp_check;

	len = word_len(s + i, '|');
	if (check_all_redir((s + i), len) == 0)
		return (NULL);
	count = 0;
	pos = i;
	while (pos < len + i)
	{
		if (s[pos] == '"' || s[pos] == '\'')
			return (NULL);
		pos++;
	}
	exp_check = mini_strndup(s + i, word_len(s + i, '|'));
	data = get_data();
	data->valid = 0;
	ft_fprintf(2, "%s %s\n", mini_join(MS, exp_check), AMBG);
	return ("");
}

int	check_all_redir(char *s, size_t	len)
{
	char	*exp;
	size_t	pos;
	size_t	explen;

	pos = 0;
	while (pos < len && s[pos])
	{
		explen = key_len(s + pos);
		exp = find_export(mini_strndup(s + pos, explen));
		if (ft_strcmp("", exp) != 0)
			return (0);
		pos += key_len(s + pos);
	}
	return (1);
}
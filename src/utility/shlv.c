/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:35:51 by erantala          #+#    #+#             */
/*   Updated: 2025/07/14 19:51:43 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	increase_shell_lvl(void)
{
	char	*level;
	char	*res;
	int		nbr;

	level = find_export("SHLVL");
	if (ft_strcmp(level, "") == 0)
		nbr = 0;
	else
		nbr = ft_atoi(level);
	nbr++;
	if (nbr > 999)
	{
		ft_fprintf(2, SHLVL, nbr);
		replace_export("SHLVL=1");
		return ;
	}
	if (nbr < 0)
		nbr = 0;
	level = ft_itoa(nbr);
	res = mini_join("SHLVL=", level);
	free(level);
	replace_export(res);
}

char	*mini_join(char const *s1, char const *s2)
{
	size_t	i;
	char	*jstr;

	if (!s1 || !s2)
		return (NULL);
	jstr = arena_malloc((sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1)));
	i = 0;
	while (*s1)
		jstr[i++] = *(char *)s1++;
	while (*s2)
		jstr[i++] = *(char *)s2++;
	jstr[i] = '\0';
	return (jstr);
}

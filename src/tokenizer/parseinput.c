/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseinput.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:38:10 by erantala          #+#    #+#             */
/*   Updated: 2025/06/18 18:56:03 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	create_token(char *s)
{
	t_token	*new;

	new = arena_malloc(sizeof(t_token));
	
}

t_vector	*token_vector(char *s)
{
	int	i;
	t_vector	*tokens;

	tokens = new_vector(4);
	i = 0;
	while (s[i])
	{
		while(ft_isspace(s[i]))
			i++;
		create_token(s + i);
	}
}
t_vector	*token_string(char	*s)
{
	char	*s;
	int		pos;
	int		len;


}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_atoi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:22:39 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/11 22:01:52 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_atoi(const char *nptr)
{
	long	result;
	int		neg;

	result = 0;
	neg = 1;
	while (*nptr == ' ' || (*nptr >= '\a' && *nptr <= '\r'))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			neg = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + (*nptr - '0');
		if (result > INT_MAX)
			break ;
		nptr++;
	}
	if (result > INT_MAX)
		return (-(result - INT_MAX));
	return ((int)(result *= neg));
}

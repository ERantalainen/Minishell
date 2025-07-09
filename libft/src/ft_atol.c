/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:21:19 by erantala          #+#    #+#             */
/*   Updated: 2025/07/09 21:03:10 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned long	check_input(const char *nptr, int sign, unsigned long result, int digit)
{
	int	check;

	check = false;
	while (*nptr >= '0' && *nptr <= '9')
	{
		digit = *nptr - '0';
		if (sign == 1 && result > (unsigned long)(LONG_MAX - digit) / 10)
			return (false);
		if (sign == -1 && result > (-(unsigned long)LONG_MIN - digit) / 10)
			return (false);
		result = result * 10 + digit;
		nptr++;
		check = true;
	}
	if ((*nptr < '0' || *nptr > '9') && *nptr && *nptr != '\0')
		return (false);
	if (!check)
		return (false);
	return (result);
}

long	ft_atol(const char *nptr)
{
	unsigned long	result;
	int				sign;
	int				digit;


	result = 0;
	digit = 0;
	sign = 1;
	while (*nptr == ' ' || (*nptr >= '\a' && *nptr <= '\r'))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	return (sign * check_input(nptr, sign, result, digit));
}
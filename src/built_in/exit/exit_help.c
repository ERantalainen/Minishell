/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 01:32:56 by erantala          #+#    #+#             */
/*   Updated: 2025/07/13 02:09:17 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static bool	overflow(long long nbr, int neg, int digit, bool *valid);

long	exit_atoi(const char *nptr, bool *valid)
{
	long long	res;
	int			neg;

	res = 0;
	neg = 1;
	while ((ft_isspace(*nptr)))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			neg *= -1;
		nptr++;
	}
	while (*nptr && ft_isdigit(*nptr))
	{
		if (!overflow(res, neg, (*nptr - '0'), valid))
			return (0);
		res = res * 10 + (*nptr - '0');
		nptr++;
	}
	*valid = true;
	if (!(*nptr))
		return ((long)(res * neg));
	*valid = false;
	return (0);
}

static bool	overflow(long long nbr, int neg, int digit, bool *valid)
{
	if (neg < 0)
	{
		nbr = -nbr;
		if (nbr < (((LONG_MIN) + digit) / 10))
		{
			*valid = false;
			return (false);
		}
	}
	if (neg > 0)
		if (nbr > (LONG_MAX - digit) / 10)
		{
			*valid = false;
			return (false);
		}
	return (true);
}

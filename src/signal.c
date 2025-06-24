/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 17:07:20 by erantala          #+#    #+#             */
/*   Updated: 2025/06/24 17:29:00 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t g_sig = 0;

void	sig_handler(int sig, siginfo_t *null, void *empty)
{
	(void)null;
	(void)empty;
	g_sig = sig;
}

void	sig_catcher()
{
	struct sigaction	s_sig;

	s_sig.sa_sigaction = &sig_handler;
	if (sigemptyset(&s_sig.sa_mask) == -1)
		exit (1);
	if (sigaddset(&s_sig.sa_mask, SIGINT) == -1)
		exit (1);
	if (sigaddset(&s_sig.sa_mask, EOF) == -1)
		exit (1);
	while (1)
		pause();
}
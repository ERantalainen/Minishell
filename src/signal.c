/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 17:07:20 by erantala          #+#    #+#             */
/*   Updated: 2025/06/25 17:04:05 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t g_sig = 0;

void	handler(int sig, siginfo_t *info, void *empty)
{
	(void)info;
	(void)empty;
	g_sig = sig;
	if (sig == SIGINT)
	{
		g_sig = SIGINT;
		printf("\n");
		rl_done = 1;
		rl_on_new_line();
		rl_redisplay();
	}
}

void	catcher()
{
	struct sigaction	s_sig;

	s_sig.sa_sigaction = &handler;
	if (sigemptyset(&s_sig.sa_mask) == -1)
		exit (3);
	if (sigaddset(&s_sig.sa_mask, SIGINT) == -1)
		exit (4);
	if (sigaction(SIGINT, &s_sig, NULL) == -1)
		exit(2);
	return ;
}
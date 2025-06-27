/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 17:07:20 by erantala          #+#    #+#             */
/*   Updated: 2025/06/27 15:21:41 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t g_sig = 0;

void	handler(int sig, siginfo_t *a, void *b)
{
	(void)a;
	(void)b;
	g_sig = sig;
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	catcher()
{
	struct sigaction	s_sig;
	struct sigaction	ign;

	ft_memset(&s_sig, 0, sizeof(sigaction));
	ft_memset(&ign, 0, sizeof(sigaction));
	ign.sa_handler = SIG_IGN;
	s_sig.sa_flags = SA_SIGINFO;
	s_sig.sa_sigaction = &handler;
	sigaction(SIGINT, &s_sig, NULL);
	sigaction(SIGQUIT, &ign, NULL);
	rl_event_hook = NULL;
	return ;
}

void	ignore()
{
	struct sigaction	s_sig;

	ft_memset(&s_sig, 0, sizeof(s_sig));
	s_sig.sa_handler = SIG_IGN;
	sigaction(SIGINT, &s_sig, NULL);
	sigaction(SIGQUIT, &s_sig, NULL);
	return ;
}

void	reset_sig()
{
	struct sigaction	s_sig;

	ft_memset(&s_sig, 0, sizeof(s_sig));
	s_sig.sa_handler = SIG_DFL;
	sigaction(SIGINT, &s_sig, NULL);
	sigaction(SIGQUIT, &s_sig, NULL);
	return ;
}

void	heredoc_handler(int sig)
{
	if (sig == SIGINT)
		g_sig = SIGINT;
}

int	check_signal(void)
{
	if (g_sig == SIGINT)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

void	heredoc_signal(void)
{
	struct sigaction	s_sig;

	ft_memset(&s_sig, 0, sizeof(s_sig));
	s_sig.sa_handler = heredoc_handler;
	sigaction(SIGINT, &s_sig, NULL);
	rl_event_hook = &check_signal;
}


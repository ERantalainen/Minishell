/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utility.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:50:24 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/11 22:52:49 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_signal(void)
{
	if (g_sig == SIGINT)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

static void	heredoc_handler(int sig)
{
	if (sig == SIGINT)
		g_sig = SIGINT;
}

void	heredoc_signal(void)
{
	struct sigaction	s_sig;

	ft_memset(&s_sig, 0, sizeof(s_sig));
	s_sig.sa_handler = heredoc_handler;
	sigaction(SIGINT, &s_sig, NULL);
	rl_event_hook = &check_signal;
	replace_export("?=130");
}

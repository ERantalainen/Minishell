/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utility.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:50:24 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/13 17:52:16 by erantala         ###   ########.fr       */
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

char	*here_eof(char *limiter)
{
	char	*eof_msg;
	t_data	*data;

	data = get_data();
	eof_msg = mini_join(EOF1, ft_itoa(data->line));
	limiter = mini_join(limiter, "\')");
	eof_msg = mini_join(eof_msg, EOF2);
	eof_msg = mini_join(eof_msg, limiter);
	return (eof_msg);
}

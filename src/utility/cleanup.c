/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:07:45 by erantala          #+#    #+#             */
/*   Updated: 2025/07/23 18:02:59 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_heredoc(void)
{
	size_t	i;
	t_data	*data;

	data = get_data();
	i = 0;
	while (i < data->heredocs->count)
	{
		safe_close(&data->hdfd[i]);
		unlink((char *)data->heredocs->data[i]);
		data->hdfd[i] = -1;
		i++;
	}
	data->heredocs = new_vector(2);
}

void	ft_exit(char *s, unsigned char code)
{
	t_data	*data;
	size_t	i;
	int		*fd;

	i = 0;
	data = get_data();
	close(data->stdin);
	rl_clear_history();
	while (i < data->fds->count)
	{
		fd = (int *)data->fds->data[i];
		safe_close(&*fd);
		i++;
	}
	if (!s || ft_strcmp("", s) != 0)
		ft_fprintf(STDERR_FILENO, "%s\n", s);
	free_arenas();
	exit(code);
}

void	ft_exit_child(t_pipedata *p, char *s, int code)
{
	if (p->infile >= 0)
		safe_close(&p->infile);
	if (p->outfile >= 0)
		safe_close(&p->outfile);
	if (p->stdin_copy >= 0)
		safe_close(&p->stdin_copy);
	if (p->stdout_copy >= 0)
		safe_close(&p->stdout_copy);
	if (s)
		ft_fprintf(2, "error: %s\n", s);
	free_arenas();
	exit(code);
}

void	child_died(int status)
{
	const char	*exit_code = "?=";
	char		*exit_export;

	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT || WTERMSIG(status) == SIGINT)
		{
			if (WTERMSIG(status) == SIGQUIT)
				ft_fprintf(STDERR_FILENO, "%s", QUIT);
			write(1, "\n", 1);
			rl_replace_line("", 0);
			rl_done = 1;
		}
		exit_export = mini_join(exit_code, mini_itoa(WTERMSIG(status) + 128));
		replace_export(exit_export);
	}
	else
		replace_export(mini_join(exit_code, mini_itoa(WEXITSTATUS(status))));
}

void	soft_exit(char *s, int code, bool err)
{
	t_data	*data;

	data = get_data();
	data->valid = 0;
	if (s && !err)
		ft_fprintf(2, "%s\n", s);
	if (err && s)
		perror(mini_join(MS, s));
	replace_export(mini_join("?=", mini_itoa(code)));
}

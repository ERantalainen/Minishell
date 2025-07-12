/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:44:11 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/13 00:35:45 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Writes to heredoc tmp file until LIMITER is encountered

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

static void	write_to_tmpfile(char *limiter, int index)
{
	char	*input;
	t_data	*data;

	data = get_data();
	while (true)
	{
		if (!isatty(0))
			input = get_next_line(STDIN_FILENO);
		else
			input = readline("\1\e[38;5;231m\2╰─❯❯ \1\e[0m\2");
		if (g_sig == SIGINT || !input)
		{
			if (g_sig == SIGINT)
				data->valid = 0;
			else if (!input)
				ft_fprintf(2, "%s\n", here_eof(limiter));
			return ;
		}
		if (ft_strcmp(input, limiter) == 0)
			break ;
		input = mini_join(input, "\n");
		if (write(data->hdfd[index], input, ft_strlen(input)) < 0)
			return (soft_exit("Heredoc input error", 1, 0));
	}
}

// Heredoc execution, takes the limiter and index of heredoc (if multiple)

// Heredoc loop

void	here_loop(char *limiter, int index, t_data *data, char *name)
{
	add_elem(data->fds, &data->hdfd[index]);
	heredoc_signal();
	write_to_tmpfile(limiter, index);
	catcher();
	if (g_sig != 0)
		g_sig = 0;
	if (data->hdfd[index] >= 0)
		close(data->hdfd[index]);
	data->hdfd[index] = open(name, O_RDONLY);
	if (data->hdfd[index] == -1)
		soft_exit("heredoc", 1, 1);
	add_elem(data->heredocs, name);
}

char	*here_doc(char *limiter, int index, t_type type)
{
	const char	*base = "._heredoc_.";
	static int	filecount = 0;
	char		*name;
	t_data		*data;

	data = get_data();
	while (access(name_join(base, mini_itoa(filecount)), F_OK) == 0)
		filecount++;
	name = name_join(base, mini_itoa(filecount));
	data->hdfd[index] = open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (data->hdfd[index] == -1)
		soft_exit("heredoc", 1, 1);
	here_loop(limiter, index, data, name);
	if (type == STRING && data->valid)
		here_check(data->hdfd[index], name, data, index);
	return (name);
}

char	*name_join(char const *s1, char const *s2)
{
	size_t	i;
	char	*jstr;

	if (!s1 || !s2)
		return (NULL);
	jstr = arena_malloc((sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1)));
	i = 0;
	while (*s1)
		jstr[i++] = *(char *)s1++;
	while (*s2)
		jstr[i++] = *(char *)s2++;
	jstr[i] = '\0';
	return (jstr);
}

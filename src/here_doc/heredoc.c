/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:44:11 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/13 23:32:40 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*here_line(char *input)
{
	char	*temp;

	if (!isatty(STDIN_FILENO))
		input = get_next_line(STDIN_FILENO);
	else
		input = readline("\1\e[38;5;231m\2❯❯ \1\e[0m\2");
	if (!input)
		return (NULL);
	if (input[ft_strlen(input) - 1] == '\n')
		temp = mini_strndup(input, ft_strlen(input) - 1);
	else
		temp = mini_strdup(input);
	free(input);
	return (temp);
}

static void	write_to_tmpfile(char *limiter, int index, t_data *data)
{
	char	*input;

	input = "";
	while (true)
	{
		input = here_line(input);
		if (g_sig == SIGINT || (!input && isatty(0)))
		{
			if (g_sig == SIGINT)
				data->valid = 0;
			else if (!input)
				ft_fprintf(2, "%s\n", here_eof(limiter));
			break ;
		}
		if (!input || ft_strcmp(input, limiter) == 0)
			break ;
		input = mini_join(input, "\n");
		if (write(data->hdfd[index], input, ft_strlen(input)) < 0)
			return (free(input), soft_exit("Heredoc input error", 1, 0));
	}
}

// Heredoc execution, takes the limiter and index of heredoc (if multiple)
// Heredoc loop
void	here_loop(char *limiter, int index, t_data *data, char *name)
{
	add_elem(data->fds, &data->hdfd[index]);
	heredoc_signal();
	write_to_tmpfile(limiter, index, data);
	catcher();
	if (g_sig != 0)
		g_sig = 0;
	if (data->hdfd[index] >= 0)
		safe_close(&data->hdfd[index]);
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

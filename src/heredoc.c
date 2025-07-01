/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:44:11 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/01 15:44:28 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Writes to heredoc tmp file until LIMITER is encountered

char	*here_eof(char *limiter)
{
	char		*eof_msg;
	t_data		*data;

	data = get_data();
	eof_msg = mini_join(EOF1, ft_itoa(data->line));
	limiter = mini_join(limiter, "\')");
	eof_msg = mini_join(eof_msg, EOF2);
	eof_msg = mini_join(eof_msg, limiter);
	return (eof_msg);
}

static void	write_to_tmpfile(t_vector *tokens, char *limiter, int index)
{
	char	*input;
	t_data	*data;

	(void)tokens;
	data = get_data();
	while (true)
	{
		input = readline("\001\033[1;32m\002heredoc>\001\033[0m\002 ");
		if (g_sig == SIGINT || !input)
		{
			if (g_sig == SIGINT)
				data->valid = 0;
			else if (!input)
				ft_fprintf(STDERR_FILENO, "%s\n", here_eof(limiter));
			return ;
		}
		if (ft_strcmp(input, limiter) == 0)
			break ;
		input = name_join(input, "\n");
		if (write(data->hdfd[index], input, ft_strlen(input)) < 0)
			exit(1);
	}
}

// Heredoc execution, takes the limiter and index of heredoc (if multiple)
char	*here_doc(t_vector *tokens, char *limiter, int index)
{
	const char	*base = "._heredoc_.";
	static int	filecount = 0;
	char		*name;
	t_data		*data;

	data = get_data();
	while (access(name_join(base, ft_itoa(filecount)), F_OK) == 0)
		filecount++;
	name = name_join(base, ft_itoa(filecount));
	data->hdfd[index] = open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (data->hdfd[index] < 0)
		exit(1);
	add_elem(data->fds, &data->hdfd[index]);
	heredoc_signal();
	write_to_tmpfile(tokens, limiter, index);
	catcher();
	if (g_sig != 0)
		g_sig = 0;
	close(data->hdfd[index]);
	data->hdfd[index] = open(name, O_RDONLY);
	if (data->hdfd[index] < 0)
		exit(1);
	add_elem(data->heredocs, name);
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

static char	ft_itod(int n)
{
	if (n > 9)
	{
		ft_itod(n / 10);
		n = n % 10;
	}
	return (n + '0');
}

static int	ft_intlen(int n)
{
	size_t	temp;
	size_t	i;

	i = 0;
	temp = 0;
	if (n == INT_MIN)
		return (10);
	if (n < 0)
	{
		temp = -n;
		i++;
	}
	else
		temp = n;
	while (temp > 9 && i <= (size_t)n)
	{
		temp /= 10;
		i++;
	}
	return (i);
}

char	*mini_itoa(int n)
{
	int		i;
	char	*result;
	int		neg;

	neg = 0;
	i = ft_intlen(n) + 1;
	result = arena_malloc(sizeof(char) * (i + 1));
	if (!result)
		return (NULL);
	if (n == INT_MIN)
		return (ft_strlcpy(result, "-2147483648", 12), result);
	result[i] = '\0';
	if (n < 0)
	{
		result[0] = '-';
		if (n < 0 && i--)
			n = -n;
		neg = 1;
	}
	while (i--)
	{
		result[i + neg] = ft_itod(n);
		n /= 10;
	}
	return (result);
}

char	*here_expand(int fd, t_data *data)
{
	char		**file;
	char		*line;
	t_vector	*lines;
	size_t	i;
	size_t	j;

	i = 0;
	lines = new_vector(4);
	while(1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		add_elem(lines, line);
	}
	file = vec_to_array(lines);
	while (file[i])
	{
		while (file[i][j++])
			if (file[i][j] == '$')
		i++;
	}

	return (line);
}

char	*make_here_expand(char *ln, size_t i)
{
	char	*expansion;
	size_t	len;
	size_t	expan_len;

	expansion = (find_export(mini_strndup((ln + i), word_len(ln + i))));
	len = ft_strlen(ln);
	expan_len = ft_strlen(expansion);
	expansion = mini_join(mini_strndup(ln, i - 1), expansion);
	if (len - expan_len > 0)
	{
		mini_join(expansion, ln + (len - expan_len));
	}
	return (expansion);
}
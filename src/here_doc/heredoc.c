/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:44:11 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/03 15:52:32 by erantala         ###   ########.fr       */
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

static char	*get_heredoc_prompt(void)
{
    char *prompt;
    
    prompt = arena_malloc(128);
    ft_strlcpy(prompt, "\033[38;5;231m╰─❯❯ \033[0m", 128);
    return (prompt);
}

static void	write_to_tmpfile(char *limiter, int index)
{
	char	*input;
	t_data	*data;

	data = get_data();
	if (data->non_interactive == true)
		return ;
	while (true)
	{
		input = readline(get_heredoc_prompt());
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
			exit(1);
	}
}

// Heredoc execution, takes the limiter and index of heredoc (if multiple)
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
	if (data->hdfd[index] < 0)
		exit(5);
	add_elem(data->fds, &data->hdfd[index]);
	heredoc_signal();
	write_to_tmpfile(limiter, index);
	catcher();
	if (g_sig != 0)
		g_sig = 0;
	close(data->hdfd[index]);
	data->hdfd[index] = open(name, O_RDONLY);
	if (type == STRING)
		here_check(data->hdfd[index], name, data, index);
	if (data->hdfd[index] < 0)
		exit(6);
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
void	here_check(int fd, char *name, t_data *data, size_t index)
{
	char		**file;
	char		*line;
	t_vector	*lines;
	size_t		i;
	size_t		j;

	i = 0;
	lines = new_vector(4);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		add_elem(lines, line);
	}
	file = vec_to_array(lines);
	while (file[i])
	{
		j = 0;
		while (file[i][j])
		{
			if (file[i][j] == '$')
				file[i] = here_expansion(file[i], &j);
			j++;
		}
		i++;
	}
	fix_lines(file, index, name, data);
}

// write new heredoc file.

char	*here_expansion(char *ln, size_t *i)
{
	char	*expansion;
	size_t	len;
	size_t	expan_len;

	expansion = (find_export(mini_strndup((ln + (*i)), word_len(ln + (*i),
						'"'))));
	len = ft_strlen(ln);
	expan_len = word_len(ln + (*i), '"');
	expansion = mini_join(mini_strndup(ln, (*i)), expansion);
	(*i) += expan_len;
	if ((*i) < len)
		expansion = mini_join(expansion, ln + (*i));
	return (expansion);
}

void	fix_lines(char **file, size_t index, char *name, t_data *data)
{
	int		fd;
	size_t	i;

	fd = data->hdfd[index];
	i = 0;
	close(fd);
	fd = open(name, O_WRONLY);
	if (fd == -1)
		perror("minishell:");
	while (file[i])
	{
		write(fd, file[i], ft_strlen(file[i]));
		i++;
	}
	close(fd);
	fd = open(name, O_RDONLY);
	if (fd == -1)
		perror("minishell:");
	data->hdfd[i] = fd;
}

char	*mini_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	i;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	res = arena_malloc(len + 1);
	i = 0;
	while (i < len)
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

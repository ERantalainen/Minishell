/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:44:11 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/24 01:47:00 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Writes to heredoc tmp file until LIMITER is encountered
static void	write_to_tmpfile(t_vector *tokens, char *limiter, int index)
{
	char	*input;
	t_data	*data;

	(void)tokens;
	data = get_data();
	while (true)
	{
		printf("%s\n", limiter);
		input = readline("heredoc>");
		if (!input)
			break ;
		if (ft_strcmp(input, limiter) == 0)
		{
			free(input);
			break ;
		}
		if (write(data->hdfd[index], input, ft_strlen(input)) < 0)
			exit(1);
		free(input);
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
	write_to_tmpfile(tokens, limiter, index);
	close(data->hdfd[index]);
	data->hdfd[index] = open(name, O_RDONLY);
	if (data->hdfd[index] < 0)
		exit(1);
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
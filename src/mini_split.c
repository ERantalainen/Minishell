/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:55:30 by erantala          #+#    #+#             */
/*   Updated: 2025/07/01 19:26:03 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_get_word_len(const char *arr, char c)
{
	size_t	size;
	size_t	i;

	size = 0;
	i = 0;
	while (arr[i] != '\0' && arr[i] == c)
		i++;
	while (arr[i] != '\0' && arr[i] != c)
	{
		size++;
		i++;
	}
	return (size);
}

static size_t	ft_words_in_s(const char *s, char c)
{
	size_t	count;
	size_t	i;

	if (c == '\0')
	{
		if (*s != '\0')
			return (1);
		else
			return (0);
	}
	count = 0;
	i = 0;
	if (s[i] && s[i + 1] && s[i] != c)
		count++;
	while (s[i])
	{
		if (s[i] == c && s[i + 1] && s[i + 1] != c)
			count++;
		i++;
	}
	return (count);
}

static char	*ft_find_char(const char **arr, char c)
{
	char	*buffer;
	size_t	i;

	while (**arr != '\0' && **arr == c)
		(*arr)++;
	i = ft_get_word_len(*arr, c);
	buffer = arena_malloc(i + 1);
	i = 0;
	while (**arr != '\0' && **arr != c)
	{
		buffer[i] = **arr;
		i++;
		(*arr)++;
	}
	buffer[i] = '\0';
	while (**arr != '\0' && **arr == c)
		(*arr)++;
	return (buffer);
}

char	**mini_split(const char *s, char c)
{
	char	**result;
	size_t	i;
	size_t	word_count;

	if (!s)
		return (NULL);
	word_count = ft_words_in_s(s, c);
	result = arena_malloc((word_count + 1) * sizeof(char *));
	i = 0;
	while (*s && i < word_count)
	{
		result[i] = ft_find_char(&s, c);
		i++;
	}
	result[i] = NULL;
	return (result);
}

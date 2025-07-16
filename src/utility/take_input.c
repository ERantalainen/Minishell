/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 02:49:43 by erantala          #+#    #+#             */
/*   Updated: 2025/07/17 00:31:08 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*color_path(char *path)
{
	char	*result;
	char	char_str[2];
	int		i;

	result = mini_strdup("");
	char_str[1] = '\0';
	i = 0;
	while (path[i])
	{
		if (path[i] == '/')
			result = mini_join(result, "\1\e[38;5;231m\2/");
		else
		{
			if (i == 0 || path[i - 1] == '/')
				result = mini_join(result, "\1\e[38;5;219m\2");
			char_str[0] = path[i];
			result = mini_join(result, char_str);
		}
		i++;
	}
	return (result);
}

static char	*get_prompt(void)
{
	char	*cwd;
	char	*colored_cwd;
	char	*prompt;
	t_data	*data;

	data = get_data();
	cwd = data->directory;
	colored_cwd = color_path(cwd);
	prompt = mini_join("\1\e[38;5;231m\2❯ ", colored_cwd);
	prompt = mini_join(prompt, " \1\e[38;5;231m\2❯ \1\e[38;5;156m\2minishell"
			"\1\e[0m\2\1\e[38;5;231m\2 ❯\1\e[0m\2 ");
	return (prompt);
}

char	*take_input(t_data *data)
{
	char	*input;
	char	*prompt;
	char	*ret;

	prompt = get_prompt();
	while (1)
	{
		if (!isatty(STDIN_FILENO))
				dup2(data->stdin, STDIN_FILENO);
		input = readline(prompt);
		if (!input)
			ft_exit("exit", 0);
		if (input[0] != 0)
		{
			ret = mini_strdup(input);
			free (input);
			if (check_quotes(input))
			{
				add_history(ret);
				replace_export("?=2");
				return (NULL);
			}
			return (ret);
		}
	}
}

int	check_quotes(char *s)
{
	int		i;
	char	quote;

	i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			quote = s[i++];
			while (s[i] && s[i] != quote)
				i++;
			if (!s[i])
			{
				ft_fprintf(2, "minishell: error unclosed quotes\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	pwd_check(void)
{
	char	*path;
	int		len;

	len = 50;
	path = NULL;
	while (!path)
	{
		path = arena_malloc(len);
		path = getcwd(path, len);
		if (!path && errno == ERANGE)
			path += 25;
		else if (!path && errno == ENOENT)
			return (0);
	}
	return (1);
}

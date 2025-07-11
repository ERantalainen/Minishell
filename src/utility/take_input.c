/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 02:49:43 by erantala          #+#    #+#             */
/*   Updated: 2025/07/11 22:22:05 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_prompt(void)
{
	char	*cwd;
	char	*prompt;
	char	*temp;
	t_data	*data;

	data = get_data();
	cwd = data->directory;
	prompt = mini_join("\1\e[38;5;231m\2╭─❮ \1\e[38;5;219m\2", cwd);
	temp = prompt;
	prompt = mini_join(temp, " \1\e[38;5;231m\2❯ \1\e[38;5;156m\2minishell"
			"\1\e[0m\2\n\1\e[38;5;231m\2╰─❯\1\e[0m\2 ");
	return (prompt);
}

char	*take_input(void)
{
	char	*input;
	char	*ret;

	while (1)
	{
		input = readline(get_prompt());
		if (!input)
		{
			ft_exit("exit", 0);
		}
		if (input[0] != 0)
		{
			ret = mini_strdup(input);
			if (check_quotes(input))
			{
				add_history(ret);
				free(input);
				replace_export("?=2");
				return (NULL);
			}
			free(input);
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
				ft_fprintf(2, "Syntax error unclosed quotes\n");
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

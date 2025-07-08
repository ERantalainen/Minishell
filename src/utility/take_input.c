/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 02:49:43 by erantala          #+#    #+#             */
/*   Updated: 2025/07/08 17:34:08 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_prompt(void)
{
	char	*cwd;
	char	*prompt;
	char	*temp;

	cwd = get_pwd();
	prompt = arena_malloc(256);
	prompt = mini_join("\033[38;5;231m╭─❮ \033[38;5;219m", cwd);
	temp = prompt;
	prompt = mini_join(temp,
			" \033[38;5;231m❯ \033[38;5;156mminishell\033[0m\n\033[38;5;231m╰─❯ \033[0m");
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
	bool	syntax;
	char	quote;

	syntax = 1;
	i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			quote = s[i];
			i++;
			syntax = 0;
			while (s[i] && s[i] != quote)
				i++;
			if (!s[i])
				break ;
			syntax = 1;
		}
		i++;
	}
	if (syntax == 0)
	{
		ft_fprintf(2, "Syntax error unclosed quotes\n");
		return (1);
	}
	return (0);
}

void	pwd_check()
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
		else if (errno == ENOENT)
			chdir()
	}
}
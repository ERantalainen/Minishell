/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:39:37 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/11 21:57:23 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_option(char *s)
{
	int	i;

	i = 0;
	if (s[i] != '-')
		return (-1);
	i++;
	while (s[i] && s[i] == 'n')
		i++;
	if (s[i] && s[i] != 'n')
		return (-1);
	else
		return (i);
}

static void	options(char *command, int *i, bool *nl)
{
	char	*check_option;
	size_t	j;

	j = 0;
	if ((command[*i] == '-' && command[*i + 1] == 'n'))
	{
		while (1 && command[*i + j])
		{
			check_option = mini_strndup(command + *i + j,
					ft_strlen(command + *i + j));
			if (valid_option(check_option) == -1)
				break ;
			else
				*i += word_len(command + *i, 0) + (j - *i);
			j = *i;
			*nl = 0;
			while (ft_isspace(command[j]))
				j++;
		}
	}
}

static int	check_non_quoted_string(t_cmd *cmd)
{
	int		i;
	int		total;
	char	**split;

	split = mini_split(cmd->str, ' ');
	if (ft_stralen(split) < 2)
		return (0);
	i = 0;
	total = 0;
	while (split[i])
	{
		if (valid_option(split[i]) == -1)
			break ;
		else
			i++;
	}
	while (i - 1 >= 0)
	{
		total += ft_strlen(split[i - 1]);
		if ((i - 1) % 2 == 0)
			total++;
		i--;
	}
	return (total);
}

char	*echo_part(t_cmd *cmd, int *pos, bool *nl)
{
	char	*command;

	if (cmd->quoted == 0)
		cmd->str += check_non_quoted_string(cmd);
	command = cmd->str;
	while (command[*pos] && !cmd->quoted && ft_isspace(command[*pos]))
		(*pos)++;
	options(command, pos, nl);
	if (!command[*pos] && cmd->space == 0)
		return ("");
	else
		return (mini_strdup(command += *pos));
}

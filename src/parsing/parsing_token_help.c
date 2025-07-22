/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token_help.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 02:09:42 by erantala          #+#    #+#             */
/*   Updated: 2025/07/22 14:56:03 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*check_type(t_token *new, t_data *data, size_t *i, char *s)
{
	if (new->quoted == 1)
		return (new);
	if (s[*i - 1] == '"' || s[*i - 1] == '\'')
		new->t = STRING;
	else if (data->tokens->count == 0 && !ft_isspace(s[*i]))
		new->t = STRING;
	if (ft_strncmp(new->s, "|", 1) == 0)
		new->t = PIPE;
	else if (ft_strcmp(new->s, "<") == 0)
		new->t = INPUT;
	else if (ft_strcmp(new->s, ">") == 0)
		new->t = OUTPUT;
	else if (ft_strncmp(new->s, ">>", 2) == 0)
		new->t = APPEND;
	else
		new->t = STRING;
	if (ft_strncmp(new->s, "<<", 2) == 0
		&& (s[*i - 1] != '"' && s[*i - 1] != '\''))
		new->t = HERE_DOC;
	return (new);
}

void	first_trim_check(t_vector *commands)
{
	t_cmd	*cmd;
	size_t	i;

	i = 0;
	cmd = (t_cmd *)commands->data[1];
	if (!cmd || (cmd->type != STRING && cmd->type != FILES))
		return ;
	if (!cmd->quoted)
	{
		while (cmd->str[i] && ft_isspace(cmd->str[i]))
			i++;
	}
	else
		return ;
	cmd->str += i;
	commands->data[1] = cmd;
}

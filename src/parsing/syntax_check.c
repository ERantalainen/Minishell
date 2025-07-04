/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:48:23 by erantala          #+#    #+#             */
/*   Updated: 2025/07/04 16:53:02 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	syntax_print(char *error, t_data *data, int exit, bool per)
{
	if (per == 1)
		perror(error);
	else
		ft_fprintf(2, "%s\n", error);
	data->valid = 0;
	replace_export(mini_join("?=", mini_itoa(exit)));
}

void	check_repeat(t_vector *tokens)
{
	size_t	i;
	t_token	*cur;
	t_token	*nx;
	t_data	*data;

	data = get_data();
	i = 0;
	while (i < tokens->count - 1)
	{
		cur = tokens->data[i];
		nx = tokens->data[i + 1];
		if (cur->t == INPUT && nx->t == INPUT)
		{
			remove_elem(tokens, i);
			nx->t = HERE_DOC;
			nx->s = mini_strdup("<<");
		}
		if (cur->t == PIPE && nx->t == PIPE)
			syntax_print(mini_join(TOKEN, "|'"), data, 127, 0);
		if (data->valid == 0)
			return ;
		i++;
	}
}

// ENSURE NO SYNTAX ERROR WITH REPEAT SYMBOLS
int	check_heredoc(t_vector *tokens)
{
	size_t	i;
	size_t	count;
	t_token	*curr;
	t_token	*next;
	t_data	*data;

	data = get_data();
	count = 0;
	i = 0;
	curr = tokens->data[i];
	if (tokens->count == 1 && curr->t == HERE_DOC)
	{
		ft_fprintf(2, "%s'\n", mini_join(TOKEN, "newline"));
		replace_export("?=2");
		data->valid = 0;
		return (0);
	}
	while (i < tokens->count - 1)
	{
		curr = tokens->data[i];
		next = tokens->data[i + 1];
		if (curr->t == HERE_DOC)
		{
			if (next->t != STRING && next->t != HERE_NOEXP)
			{
				ft_fprintf(2, "%s'\n", mini_join(TOKEN, next->s));
				data->valid = 0;
				replace_export("?=2");
				return (0);
			}
			if (next->t == INPUT)
				return (0);
			count++;
		}
		i++;
	}
	if (count > 16)
		ft_exit("minishell: here document count exceeded", 2);
	if (count > 0)
	{
		data->hd_count = count;
		data->hdfd = arena_malloc(sizeof(int) * count);
	}
	i = 0;
	while (count > 0)
	{
		curr = tokens->data[i];
		next = tokens->data[i + 1];
		if ((curr->t == HERE_DOC) && next != NULL)
		{
			next->s = here_doc(next->s, count - 1, next->t);
			if (!data->valid)
				return (0);
			next->t = FILES;
			count--;
		}
		i++;
	}
	return (0);
}

static void	input_syntax(t_cmd *cmd, t_data *data)
{
	if (cmd->type == INPUT && cmd->next == EMPTY)
			data->valid = -1;
	else if (cmd->type == INPUT && cmd->next != FILES)
			data->valid = -cmd->next;
}

static void output_syntax(t_cmd *cmd, t_data *data)
{
		if (cmd->type == OUTPUT && cmd->next == EMPTY)
			data->valid = -1;
		else if (cmd->type == OUTPUT && cmd->next != STRING
			&& cmd->next != FILES)
			data->valid = -cmd->next;
}
static void appen_syntax(t_cmd *cmd, t_data *data)
{
	if (cmd->type == APPEND && cmd->next == EMPTY)
		data->valid = -1;
	else if (cmd->type == APPEND && cmd->next != STRING
		&& cmd->next != FILES)
		data->valid = -cmd->next;
}

static void	check_files(t_cmd *cmd, t_cmd *next, t_data *data)
{
	if (cmd->type == INPUT)
	{
		if (access(next->str, R_OK) != 0)
		{
			syntax_print(mini_join(MS, mini_strndup(next->str, word_len(next->str, 0))), data, 2, 1);
			return ;
		}
		cmd->next = FILES;
		next->type = FILES;
	}
	if (cmd->type == OUTPUT)
	{
		cmd->next = FILES;
		next->type = FILES;
	}
}

void	check_command_syntax(t_vector *commands, t_data *data)
{
	size_t	i;
	t_cmd	*cmd;

	i = 0;
	while (i < commands->count)
	{
		cmd = commands->data[i];
		if (commands->data[i + 1] && (cmd->next == STRING || cmd->next == FILES))
			check_files(cmd, commands->data[i + 1], data);
		if (data->valid != 1)
			return ;
		input_syntax(cmd, data);
		output_syntax(cmd, data);
		appen_syntax(cmd, data);
		if (data->valid == -1)
			ft_fprintf(2, "%s'\n", mini_join(TOKEN, "newline"));
		else if (data->valid != 1 && data->valid != -1)
		{
			cmd = commands->data[i + 1];
			ft_fprintf(2, "%s'\n", mini_join(TOKEN, cmd->str));
		}
		if (data->valid != 1)
			return ;
		i++;
	}
}



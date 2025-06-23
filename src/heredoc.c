/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:44:11 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/23 15:44:25 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Writes to heredoc tmp file until LIMITER is encountered
static void	write_to_tmpfile(t_vector *tokens, char *limiter, int index)
{
	char	*input;
	int		limiter_len;

	limiter_len = ft_strlen(limiter);
	while (true)
	{
		input = get_next_line(STDIN_FILENO);
		if (!input)
			break ;
		if (ft_strncmp(input, limiter, limiter_len) == 0 && input[limiter_len] == '\n')
		{
			free(input);
			break ;
		}
		if (write(tokens->datapool.hdfd[index], input, ft_strlen(input)) < 0)
			exit(1);
		free(input);
	}
}

// Heredoc execution, takes the limiter and index of heredoc (if multiple)
void	here_doc(t_vector *tokens, char *limiter, int index)
{
	tokens->datapool.hdfd[index] = open("._heredoc_.tmp", O_RDWR | O_CREAT | O_EXCL, 0600);
	if (tokens->datapool.hdfd[index] < 0)
		exit(1);
	write_to_tmpfile(tokens, limiter, index);
	close(tokens->datapool.hdfd[index]);
	tokens->datapool.hdfd[index] = open("._heredoc_.tmp", O_RDONLY);
	if (tokens->datapool.hdfd[index] < 0)
		exit(1);
}

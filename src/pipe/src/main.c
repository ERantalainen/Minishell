/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 22:18:10 by jpelline          #+#    #+#             */
/*   Updated: 2025/06/17 18:34:37 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	handle_open_error(char *filename, t_pipex *p)
{
	static int	check;

	if (errno == EISDIR)
		ft_printf(STDERR_FILENO, "%s: Is a directory\n", filename);
	else if (errno == ENOTDIR)
		ft_printf(STDERR_FILENO, "%s: Not a directory\n", filename);
	else if (errno == EACCES)
		ft_printf(STDERR_FILENO, "%s: Permission denied\n", filename);
	else if (errno == ENOENT)
		ft_printf(STDERR_FILENO, "%s: No such file or directory\n", filename);
	else
		ft_printf(STDERR_FILENO, "%s: Error opening\n", filename);
	if (check == 1)
		exit_handler(0, NULL, p);
	check = 1;
	if (p->outfile < 0)
		exit_handler(1, NULL, p);
}

static void	write_to_tmpfile(t_pipex *p, char **av)
{
	char	*input;
	int		len;

	len = ft_strlen(av[2]);
	while (true)
	{
		input = get_next_line(STDIN_FILENO);
		if (!input)
			break ;
		if (ft_strncmp(input, av[2], len) == 0 && input[len] == '\n')
		{
			free(input);
			break ;
		}
		if (write(p->infile, input, ft_strlen(input)) < 0)
			exit_handler(1, "write failed", p);
		free(input);
	}
}

static void	heredoc(t_pipex *p, char **av)
{
	p->is_heredoc = true;
	p->infile = open("heredoc_.txt", O_RDWR | O_CREAT | O_EXCL, 0600);
	if (p->infile < 0)
		handle_open_error(av[1], p);
	write_to_tmpfile(p, av);
	close(p->infile);
	p->infile = open("heredoc_.txt", O_RDONLY);
	if (p->infile < 0)
		handle_open_error(av[1], p);
}

static void	open_infile(t_pipex *p, char **av)
{
	p->is_heredoc = false;
	p->infile = open(av[1], O_RDONLY);
	if (p->infile < 0)
		handle_open_error(av[1], p);
}

int	main(int ac, char **av, char **env)
{
	t_pipex	*p;

	if (ac < 5)
		exit_handler(1, "Error: Invalid amount of arguments!", NULL);
	p = ft_calloc(1, sizeof(t_pipex));
	if (!p)
		exit_handler(1, "allocation failed", NULL);
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
		heredoc(p, av);
	else
		open_infile(p, av);
	if (p->is_heredoc == true)
		p->outfile = open(av[ac - 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		p->outfile = open(av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (p->outfile < 0)
		handle_open_error(av[ac - 1], p);
	else if (p->infile < 0)
		exit_handler(0, NULL, p);
	execute_pipeline(p, ac, av, env);
	exit_handler(WEXITSTATUS(p->status), NULL, p);
}

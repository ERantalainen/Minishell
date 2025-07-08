/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:12:49 by erantala          #+#    #+#             */
/*   Updated: 2025/07/08 16:00:56 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/wait.h>
# include <sys/stat.h>

# include "structs.h"
# include "functions.h"

# define ARENA_SIZE 16384
# define ALIGNMENT 8

# define EOF1 "minishell: warning: here-document at line "
# define EOF2 " delimited by end-of-file (wanted `"

# define MS "minishell: "
# define QUIT "Quit (core dumped)"
# define INV "': not a valid identifier"
# define SHLVL "minishell: warning: shell level (%d) too high, resetting to 1\n"
# define TOKEN "minishell: syntax error near unexpected token `"
# define HRLIM "minishell: maximum here-document count exceeded\n"
# define PRMD "minishell: Permission denied"
# define NSFOD ": No such file or directory"
# define AMBG ": ambiguous redirect"
# define MARG "cd: too many arguments"
# define CMD ": command not found"

extern sig_atomic_t	g_sig;

typedef struct stat s_stat;
typedef s_stat t_stat;

#endif // MINISHELL_H

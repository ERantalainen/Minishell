/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 14:12:49 by erantala          #+#    #+#             */
/*   Updated: 2025/06/26 01:08:25 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <assert.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>

# define ARENA_SIZE 16384
# define ALIGNMENT 8

enum				e_pipe
{
	READ,
	WRITE
};

typedef struct s_arena
{
	size_t			max;
	size_t			index;
	char			data[];
}					t_arena;

typedef enum e_type
{
	EMPTY,
	PIPE,
	INPUT,
	OUTPUT,
	APPEND,
	HERE_DOC,
	STRING,
	FILES,
	BUILTIN
}	t_type;

typedef struct s_token
{
	t_type			t;
	char			*s;
}					t_token;

typedef struct s_vector
{
	size_t			count;
	size_t			size;
	void			**data;
}					t_vector;

typedef struct s_variable
{
	char			*key;
	char			*expansion;
}					t_var;

typedef struct s_command
{
	char			*str;
	t_type			type;
	t_type			next;
}					t_cmd;

typedef struct s_data
{
	int			*hdfd;
	char		*directory;
	int			hd_count;
	t_cmd		*commands;
	t_vector	*envv;
	t_vector	*fds;
	char		**environ;
	t_vector	*env_vec;
	int			shell;
}	t_data;

extern sig_atomic_t	g_sig;

void				init_data(char	**env);
char				*get_pwd(void);

// Helpers

t_arena				*init_arena(size_t size);
void				*arena_malloc(size_t n);
t_arena				**get_arenas(t_arena **new);
t_arena				*find_arena(size_t n);
t_arena				**new_arena(t_arena **curr, int count, size_t n);

// Memory arena

t_vector			*new_vector(size_t elem);
void				expand_vector(t_vector *vector);
void				add_elem(t_vector *vector, void *elem);
void				change_data(t_vector *vector, void *elem, void *target);
void				array_to_vec(t_vector *vec, void **arr);
char				**vec_to_array(t_vector *vec);

// Vectors

char				*take_input(void);
int					check_quotes(char *s);

t_token				*create_token(char *s, size_t *i);
t_vector			*token_vector(char *s);
char				*token_string(char *s, size_t *i);
t_vector			*create_commands(t_vector *tokens);
t_cmd				*make_cmd_spc(t_vector *tokens, size_t *i);
t_cmd				*make_cmd_str(t_vector *tokens, size_t *i);
size_t				expanded_length(char *s, size_t n);

int					check_specials(int c);
char				*expand_strndup(char *s, size_t n);
char				*mini_append(char *s1, char *s2);

char				*quoted_token(char *s, char quote, size_t *i);
char				*mini_strndup(char *s, size_t n);
char				*mini_strdup(char *s);
t_cmd				*check_redirect(t_cmd *cmd, t_token *token);
size_t				word_len(char *s);

int					check_heredoc(t_vector *tokens);
char				*here_doc(t_vector *tokens, char *limiter, int index);
char				*name_join(char const *s1, char const *s2);
size_t				quote_len(char *s, char quote);

// Parsing

t_data				*get_data(void);
void				free_arenas(void);
void				ft_exit(char *s, int code);

// pipe
char				*get_bin_path(char *cmd, char **env);
char				**get_cmd_args(char *cmd, char *path);

// Built Ins

void	built_in(t_cmd *cmd);
void	build_handler(t_vector	*cmds);
void	echo(t_vector *commands, int i);
void	pwd(void);
void	unset(char	*key);
void	make_export(t_vector *cmds, size_t i);
void	cd(t_cmd *cmd);
void	env(void);

void	export(char *export);
char	*find_export(char *key);
size_t	key_len(char *s);
void	make_export(t_vector *cmds, size_t i);
void	replace_export(char *key);

void	increase_shell_lvl();
char	*mini_join(char const *s1, char const *s2);

// Signals

void				catcher();

// Signals

void				catcher(void);

#endif

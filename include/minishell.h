/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:05:32 by erantala          #+#    #+#             */
/*   Updated: 2025/06/23 15:53:08 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>

# define ARENA_SIZE 16384
# define ALIGNMENT 8

typedef struct s_arena
{
	size_t	max;
	size_t	index;
	char	data[];
}	t_arena;

// typedef	struct s_data
// {
// 	/* data */
// }	t_data;
// MAKE A STRUCT TO STORE ALL DATA FDS, COMMANDS ETC.s


typedef enum e_type
{
	EMPTY,
	PIPE,
	INPUT,
	OUTPUT,
	APPEND,
	HERE_DOC,
	STRING,
	FILE
}	t_type;

typedef	struct s_token
{
	t_type	t;
	char	*s;
}	t_token;

typedef	struct s_vector
{
	size_t	count;
	size_t	size;
	void	**data;
}	t_vector;

typedef	struct s_variable
{
	char	*key;
	char	*expansion;
}	t_var;

typedef struct s_command
{
	char	*str;
	t_type	type;
	t_type	next;
} t_cmd;


t_arena 	*init_arena(size_t size);
void		*arena_malloc(size_t n);
t_arena 	**get_arenas(void);
t_arena 	*find_arena(size_t n);
t_arena		**new_arena(t_arena **curr, int	count, size_t n);

t_vector	*new_vector(size_t elem);
void		expand_vector(t_vector **vector, size_t elems);
int			add_elem(t_vector *vector, void *elem);
void		change_data(t_vector *vector, void *elem, void *target);

char		*take_input();
int			check_quotes(char *s);

t_token		*create_token(char *s, size_t *i);
t_vector	*token_vector(char *s);
char		*token_string(char	*s, size_t	*i);
t_vector	*create_commands(t_vector *tokens);
t_cmd		*make_cmd_spc(t_vector *tokens, size_t *i);
t_cmd		*make_cmd_str(t_vector *tokens, size_t *i);
size_t		expanded_length(char *s, size_t n);

int			check_specials(int	c);
char		*expand_strndup(char *s, size_t n);
char		*mini_append(char *s1, char *s2);

char		*quoted_token(char *s, char quote, size_t *i);
char		*mini_strndup(char *s, size_t n);
size_t		word_len(char *s);

char		*find_export(char *key);
void		export(char *key, char *expansion);
t_vector	*get_vars();

#endif

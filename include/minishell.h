/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:05:32 by erantala          #+#    #+#             */
/*   Updated: 2025/06/19 19:33:20 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include "libft.h"

# define ARENA_SIZE 16384
# define ALIGNMENT 8

typedef struct s_arena
{
	size_t	max;
	size_t	index;
	char	data[];
}	t_arena;

typedef enum e_type
{
	EMPTY,
	PIPE,
	INPUT,
	OUTPUT,
	APPEND,
	HERE_DOC,
	STRING
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

t_token		*create_token(char *s, int *i);
t_vector	*token_vector(char *s);
char		*token_string(char	*s, int	*i);
t_vector	*create_commands(t_vector *tokens);
t_cmd		*make_command(t_vector *tokens, int *i);
void		mini_append(char *s1, char *s2);

char		*quoted_token(char *s, char quote, int *i);
char		*mini_strndup(char *s, size_t n);
size_t		word_len(char *s);

char		*find_export(char *key);
void		export(char *key, char *expansion);
t_vector	*get_vars();

#endif

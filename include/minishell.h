/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:05:32 by erantala          #+#    #+#             */
/*   Updated: 2025/06/18 18:54:24 by erantala         ###   ########.fr       */
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
	PIPE,
	INPUT,
	OUTPUT,
	APPEND,
	HERE_DOC
}	t_type;

typedef	struct s_token
{
	t_type	t;
	char	*s;
	int		i;
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


t_arena *init_arena(size_t size);
void	*arena_malloc(size_t n);
t_arena **get_arenas(void);
t_arena *find_arena(size_t n);
t_arena **new_arena(t_arena **curr, int	count, size_t n);

t_vector	*new_vector(size_t elem);
void		expand_vector(t_vector **vector, size_t elems);
int			add_elem(t_vector *vector, void *elem);
void		change_data(t_vector *vector, void *elem, void *target);

#endif

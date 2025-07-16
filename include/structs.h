/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:33:55 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/17 00:27:51 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

enum			e_pipe
{
	READ,
	WRITE
};

enum			e_open
{
	OUTPUT_CONF = O_WRONLY | O_TRUNC | O_CREAT,
	APPEND_CONF = O_WRONLY | O_APPEND | O_CREAT,
	INPUT_CONF = O_RDONLY
};

typedef enum e_type
{
	EMPTY,
	PIPE,
	INPUT,
	OUTPUT,
	APPEND,
	HERE_DOC,
	HERE_NOEXP,
	STRING,
	FILES,
	BUILTIN,
	QUOTED,
	EXPANSION
}				t_type;

typedef struct s_arena
{
	size_t		max;
	size_t		index;
	char		data[];
}				t_arena;

typedef struct s_token
{
	t_type		t;
	char		*s;
	bool		quoted;
	bool		space;
	bool		expansion;
}				t_token;

typedef struct s_vector
{
	size_t		count;
	size_t		size;
	void		**data;
}				t_vector;

typedef struct s_variable
{
	char		*key;
	char		*expansion;
}				t_var;

typedef struct s_command
{
	char		*str;
	t_type		type;
	t_type		next;
	bool		quoted;
	bool		space;
}				t_cmd;

typedef struct s_data
{
	int			*hdfd;
	int			line;
	char		*directory;
	char		*input;
	char		*trash;
	int			hd_count;
	int			stdin;
	t_cmd		*commands;
	t_vector	*envv;
	t_vector	*heredocs;
	t_vector	*fds;
	t_vector	*tokens;
	t_vector	*cmds;
	char		**environ;
	t_vector	*env_vec;
	t_vector	*exp;
	int			shell;
	int			valid;
	bool		non_interactive;
	bool		check_build;
	t_type		last;
}				t_data;

typedef struct s_pipedata
{
	int			stdout_copy;
	int			stdin_copy;
	int			outfile;
	int			infile;
	int			pipe_count;
	int			pipe_index;
	int			**pipefd;
	char		**cmd_args;
	int			*pids;
	size_t		cmd_index;
	int			cmd_count;
	int			index;
	bool		is_builtin;
	bool		cmd_found;
	bool		is_child;
	bool		has_out_redirect;
}				t_pipedata;

#endif // STRUCTS_H
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:21:07 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/07 19:21:08 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONS_H
# define FUNCTIONS_H

# include "structs.h"
# include <signal.h>
# include <stdbool.h>
# include <sys/types.h>


void	here_two(t_vector *tokens, int count, t_data *data);

int	here_count(t_vector *tokens, t_data *data);

void		init_data(char **env);
char		*get_pwd(void);
size_t		ft_stralen(char **s);
char		*mini_substr(char const *s, unsigned int start, size_t len);
char		**mini_split(const char *s, char c);
char		*mini_itoa(int n);
char		*ft_strornchr(const char *s, int one, int two, size_t n);

// Helpers
void		init_export(void);
void		sort_export(size_t count, t_data *data, char **exps);
void		empty_export(void);
char		**strarm(char **s, size_t i);

// Export sort
void		cmd_help(t_vector *tokens, size_t *i, t_token *token, t_cmd *cmd);
t_arena		*init_arena(size_t size);
void		*arena_malloc(size_t n);
t_arena		**get_arenas(t_arena **new);
t_arena		*find_arena(size_t n);
t_arena		**new_arena(t_arena **curr, int count, size_t n);
void		child_died(int status);

// Memory arena
t_vector	*new_vector(size_t elem);
void		expand_vector(t_vector *vector);
void		add_elem(t_vector *vector, void *elem);
void		change_data(t_vector *vector, void *elem, void *target);
void		array_to_vec(t_vector *vec, void **arr);
char		**vec_to_array(t_vector *vec);
void		remove_elem(t_vector *vector, size_t i);

// Vectors
char		*take_input(void);
int			check_quotes(char *s);

// Input
t_token		*create_token(char *s, size_t *i, t_type last, t_data *data);
t_vector	*token_vector(char *s);
char		*token_string(char *s, size_t *i, t_type last);
t_vector	*creator(char *s, size_t len, size_t i, t_data *data);

// Token creator
t_vector	*create_commands(t_vector *tokens);
void		cmd_help(t_vector *tks, size_t *i, t_token *tk, t_cmd *cm);
t_cmd		*make_cmd_spc(t_vector *tokens, size_t *i, t_data *data);
t_cmd		*make_cmd_str(t_vector *tokens, size_t *i, t_data *data);

// Command creator
size_t		expanded_length(char *s, size_t n);

int			check_specials(int c, int quote);
char		*expand_strndup(char *s, size_t n);
char		*expand_strndup(char *s, size_t n);
char		*mini_append(char *s1, char *s2);

char		*quoted_token(char *s, char quote, size_t *i, t_type last);
char		*mini_strndup(char *s, size_t n);
char		*mini_strdup(char *s);
size_t		word_len(char *s, int quote);
t_vector	*next_check(t_vector *commands);

void		check_repeat(t_vector *tokens);
size_t		quote_len(char *s, char quote);
void		check_command_syntax(t_vector *commands, t_data *data);

// Parsing

void		syntax_print(char *error, t_data *data, int exit, bool per);

// Syntax
t_data		*get_data(void);
void		free_arenas(void);
void		ft_exit(char *s, int code);

// Heredoc
void		here_check(int fd, char *name, t_data *data, size_t i);
char		*here_expansion(char *ln, size_t *i);
int			check_heredoc(t_vector *tokens);
char		*here_doc(char *limiter, int index, t_type type);
char		*name_join(char const *s1, char const *s2);
void		fix_lines(char **file, size_t i, char *name, t_data *data);

// Built Ins

void		echo(t_cmd **cmd, int i);
void		pwd(void);
void		unset(char *key);
void		cd(t_cmd **cmd, int i);
void		env(void);
void		export(char *export);
char		**export_to_arr(char *key, char **exports);
char		**unset_child(char *key, char **envi);
char		*find_export(char *key);
void		make_export(char *command);
void		replace_export(char *key);

// Built In helpers

void		child_builds(t_cmd **cmds, char **envi, int i);
void		built_in(t_cmd *cmd);
void		build_handler(t_cmd **cmds);
char		*expans_help(char *s, char *dup, size_t *i, size_t *pos);
void		increase_shell_lvl(void);
int			exit_calci(char *cmd);
char		*mini_join(char const *s1, char const *s2);
char		**ft_stradd(char **s, char *line);
char		**replace_line(char **s, char *line, size_t i);
int			find_line(char **line, char *key);
size_t		key_len(char *s);
char		**ft_strremove(char **s, int i);

// Signals
void		catcher(void);
void		ignore(void);
void		reset_sig(void);
void		here_catcher(void);
void		handler(int sig, siginfo_t *a, void *b);
void		heredoc_signal(void);

// Non interactive mode
void		non_interactive(void);
char		*get_input(char **argv, int argc);

// Execution
void		execution(t_cmd **tokens, char **env);
void		setup_pipeline(t_cmd **tokens, char **env);
char		*get_bin_path(char *cmd, char **env, t_pipedata *p);
char		**get_cmd_args(char *cmd, char *path);
bool		check_for_builtin(t_cmd **tokens);
void		find_next_cmd_index(t_cmd **tokens, t_pipedata *p);
void		wait_for_children(t_pipedata *p, int status);
void		close_unused_pipes(t_pipedata *p, int index);
void		init_pipes(t_pipedata *p);
void		open_handler(t_pipedata *p, const char *path);
void		open_file(t_cmd **tokens, t_pipedata *p, int settings);
void		check_for_redirects(t_cmd **tokens, t_pipedata *p);
void		setup_pipes(int in, int out, int close_in, int close_out);
void		setup_child(t_cmd **tokens, t_pipedata *p, char **env, int i);
int			setup_cmd_to_execute(t_cmd **tokens, t_pipedata *p);
void		child_process(t_cmd **tokens, t_pipedata *p, char **env);
char		*ambigous(char *s, size_t i);

// Cleaner
void		clean_heredoc(void);
t_vector	*next_check(t_vector *commands);
void		ft_exit_child(char *s, int code);

#endif

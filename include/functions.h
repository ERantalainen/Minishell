/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:21:07 by jpelline          #+#    #+#             */
/*   Updated: 2025/07/17 01:03:24 by jpelline         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONS_H
# define FUNCTIONS_H

# include "structs.h"

// Tokenizers
t_token		*create_token(char *s, size_t *i, t_type last, t_data *data);
t_vector	*token_vector(char *s);
char		*token_string(char *s, size_t *i, t_type *last);
t_vector	*creator(char *s, size_t len, size_t i, t_data *data);
char		*quoted_token(char *s, char quote, size_t *i, t_type *last);
char		*unquoted_expan(char *s, size_t *pos);
t_token		*check_type(t_token *new, t_data *data, size_t *i, char *s);

// Command creator
t_vector	*create_commands(t_vector *tokens);
void		first_trim_check(t_vector *commands);
t_cmd		*cmd_help(t_cmd *cmd, size_t *i, t_token *tk, t_data *data);
t_cmd		*make_cmd_spc(t_vector *tokens, size_t *i, t_data *data);
t_cmd		*make_cmd_str(t_vector *tokens, size_t *i, t_data *data);

// Input
void		non_interactive(void);
char		*get_input(char **argv, int argc);
char		*take_input(t_data *data);
int			check_quotes(char *s);

// Syntax Check
t_vector	*next_check(t_vector *commands);
void		check_repeat(t_vector *tokens);
void		check_command_syntax(t_vector *commands, t_data *data);
void		er_pr(char *error, t_data *data, int exit, bool per);
char		*ambigous(char *s, size_t i);
int			syntax_help(t_cmd *cmd, t_data *data, int i, t_vector *commands);
void		check_files(t_cmd *cmd, t_cmd *next);

// Built In
void		echo(t_cmd **cmd, int i);
void		pwd(void);
void		unset(char *key);
void		cd(t_cmd **cmd, int i);
void		env(void);
void		export(char *export);
void		built_in(t_cmd *cmd, int i);
void		build_handler(t_pipedata *p, t_cmd **cmds, int i);
void		count_unset(t_cmd **cmds, int i);
char		*join_full(t_cmd **cmds, int i);
char		*exit_join(t_cmd **cmds, int i);
long		exit_atoi(const char *nptr, bool *valid);

// Export
int			count_export(t_cmd **cmds, int i);
void		init_export(void);
void		sort_export(size_t count, t_data *data, char **exps);
void		empty_export(void);
char		*find_export(char *key);
void		make_export(char *command);
void		replace_export(char *key);
char		*expans_help(char *s, char *dup, size_t *i, size_t *pos);
char		**export_to_arr(char *key, char **exports);
char		**count_export_child(t_cmd **cmds, int i, char **envi);
char		*build_exports(t_cmd **cmds, int *i);
int			check_export(char **exports);
void		export_addition(char *command);
bool		find_exp_bool(char *key);

// Memory Arena
t_arena		*init_arena(size_t size);
void		*arena_malloc(size_t n);
t_arena		**get_arenas(t_arena **new);
t_arena		*find_arena(size_t n);
t_arena		**new_arena(t_arena **curr, int count, size_t n);

// Vector
t_vector	*new_vector(size_t elem);
void		expand_vector(t_vector *vector);
void		add_elem(t_vector *vector, void *elem);
void		change_data(t_vector *vector, void *elem, void *target);
void		array_to_vec(t_vector *vec, void **arr);
char		**vec_to_array(t_vector *vec);
void		remove_elem(t_vector *vector, size_t i);

// Helpers
void		init_data(char **env);
char		*get_pwd(void);
void		increase_shell_lvl(void);
t_data		*get_data(void);
void		clean_heredoc(void);
void		soft_exit(char *s, int code, bool err);
void		ft_exit_child(t_pipedata *p, char *s, int code);
void		free_arenas(void);
void		ft_exit(char *s, unsigned char code);
int			exit_calci(char *cmd);
int			pwd_check(void);

// Utility
size_t		expanded_length(char *s, size_t n);
int			check_specials(int c, int quote);
int			find_line(char **line, char *key);
size_t		quote_len(char *s, char quote);
char		**ft_stradd(char **s, char *line);
char		**strarm(char **s, size_t i);
char		**replace_line(char **s, char *line, size_t i);
size_t		key_len(char *s);
size_t		word_len(char *s, int quote);

// String functions
char		**ft_strremove(char **s, int i);
size_t		ft_stralen(char **s);
char		*mini_substr(char const *s, unsigned int start, size_t len);
char		**mini_split(const char *s, char c);
char		*mini_itoa(int n);
char		*expand_strndup(char *s, size_t n);
char		*mini_append(char *s1, char *s2);
char		*mini_join(char const *s1, char const *s2);
char		*expand_strndup(char *s, size_t n);
char		*ft_strornchr(const char *s, int one, int two, size_t n);
char		*mini_strndup(char *s, size_t n);
char		*mini_strdup(char *s);

// Heredoc
void		here_check(int fd, char *name, t_data *data, size_t i);
char		*here_expansion(char *ln, size_t *i);
int			check_heredoc(t_vector *tokens);
void		fix_lines(char **file, size_t i, char *name, t_data *data);
char		*here_doc(char *limiter, int index, t_type type);
char		*name_join(char const *s1, char const *s2);
void		here_two(t_vector *tokens, int count, t_data *data);
int			here_count(t_vector *tokens);

// Signal handling
void		catcher(void);
void		ignore(void);
void		reset_sig(void);
void		here_catcher(void);
void		handler(int sig, siginfo_t *a, void *b);
void		heredoc_signal(void);
char		*here_eof(char *limiter);

// Execution
void		execution(t_cmd **tokens, char **env);
void		setup_pipeline(t_cmd **tokens, char **env);
char		*get_bin_path(char *cmd, char **env, t_pipedata *p);
char		**get_cmd_args(char *cmd, char *path);
bool		check_for_builtin(t_cmd **tokens, int pipe_count);
void		find_next_cmd_index(t_cmd **tokens, t_pipedata *p);
void		wait_for_children(t_pipedata *p);
void		close_unused_pipes(t_pipedata *p, int i);
void		init_pipes(t_pipedata *p);
int			open_handler(t_pipedata *p, const char *path);
int			open_file(t_cmd **tokens, t_pipedata *p, int settings);
int			check_for_redirects(t_cmd **tokens, t_pipedata *p);
void		setup_pipes(int in, int out, int close_in, int close_out);
int			setup_child(t_cmd **tokens, t_pipedata *p, char **env, int i);
int			setup_cmd_to_execute(t_cmd **tokens, t_pipedata *p);
int			path_exists(void);
void		child_process(t_cmd **tokens, t_pipedata *p, char **env);
void		child_died(int status);
void		safe_close(int *fd);
int			safe_dup(int fd);
int			init_pipedata(t_pipedata *p);
int			check_all_redir(char *s, size_t len);
void		exit_arg_checker(char *str, t_pipedata *p);
int			echo_part(t_cmd **cmd, int *pos, bool *nl, bool qt);
void		handle_failure(t_pipedata *p, char *str);
void		close_pipe_pair(t_pipedata *p, int i);
size_t		skip_redirects(t_cmd **tokens, size_t tok_i);

#endif // FUNCTIONS_H

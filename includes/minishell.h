/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:29:31 by llage             #+#    #+#             */
/*   Updated: 2025/09/03 19:13:05 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <libft.h>
# include <ft_memory.h>
# include <ft_printf.h>
# include <get_next_line.h>
# include <signal.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <linux/limits.h>
# include <fcntl.h>
# include <ft_data.h>
# include <sys/wait.h>
# include <stdint.h>
# include <sys/stat.h>
# include <errno.h>

# ifndef DEBUG
#  define DEBUG false
# endif

# define SHELL_NAME "Eepyshell"

# define ANSI_RED "\001\033[31m\002"
# define ANSI_GREEN "\001\033[32m\002"
# define ANSI_YELLOW "\001\033[33m\002"
# define ANSI_BLUE "\001\033[34m\002"
# define ANSI_CYAN "\001\033[36m\002"
# define ANSI_RESET "\001\033[0m\002"

typedef enum e_token_type
{
	EMPTY,
	WORD,
	REDIR,
	PIPE,
}	t_token_type;

typedef enum e_status
{
	NONE,
	QUOTE,
	DOUBLE,
}	t_status;

typedef struct s_lexer
{
	char		*line;
	int			cursor;
	int			offset;
	t_status	status;
}	t_lexer;

typedef struct s_token
{
	struct s_token	*prev;
	struct s_token	*next;
	char			*value;
	t_token_type	type;
}	t_token;

typedef struct s_cmd
{
	char	**args;
	int		in_redir;
	int		out_redir;
}	t_cmd;

typedef struct s_cmd_table
{
	size_t	size;
	t_cmd	cmds[];
}	t_cmd_table;

typedef struct s_shell
{
	uint8_t	exit_status;
	t_map	env;
}	t_shell;

// --- Lexing ---

// lexer.c
t_token
*lexer(char *input);

// type_lexer.c
void
type_pipe(t_lexer *lexer);

void
type_exp(t_lexer *lexer);

void
type_word(t_lexer *lexer);

void
type_redir(t_lexer *lexer);

void
link_token_back(t_token *last_token, t_token *new_token);

t_token_type
find_type(t_lexer *lexer);

// --- Parsing ---

// parser.c
t_cmd_table
*parser(t_token **token_list, t_shell *shell);

// parse_n_count.c
int
count_pipes(t_token **token_list);

bool
parse_words(t_cmd *cmd, t_token **token_ptr);

bool
check_last(t_cmd *cmd);

// parse_redirs.c
bool
parse_redirs(t_cmd *cmd, t_token **token_ptr, t_token *token, t_shell *shell);

// parse_heredoc.c
int
parse_heredoc(t_token *token, bool last, t_shell *shell, const int previous_fd);

// pipeline.c
bool
pipe_my_line(t_cmd_table *cmd_table);

// check_gramar.c
void
check_pipe(t_token *token, t_token **token_list);

void
check_redir(t_token *token, t_token **token_list);

void
check_word(t_token *token, t_token **token_list);

// --- Expanding ---

// expand.c
char
*expand_str(char *str, t_shell *shell);

// expand_tokens.c
bool
expand_tokens(t_token **token_ptr, t_shell *shell);

// expand_utils.c
bool
is_quote(const char c);

bool
should_expand(char *line, size_t i, t_status quotes);

char
*get_expand_key(const char *str, const size_t start);

// --- Executing ---

// path.c
char
*find_in_path(t_map env, char *name);

// exec.c
void
exec_table(t_cmd_table *cmd_table, t_shell *shell);

// echo.c
int
echo(char *args[]);

// cd.c
int
cd(char *args[], t_map *env);

// cd_utils.c
void
cd_print_error(const char *name);

// pwd.c
int
pwd(char *args[]);

// export.c
int
export(char *args[], t_map *env);

// export_utils.c
int
key_cmp(const char *key_a, const char *key_b);

void
export_print_error(const char *name, const char *value, bool usage);

bool
check_options(char *args[]);

bool
is_valid_key(const char *str);

// unset.c
int
unset(char *args[], t_map *env);

// env.c
int
env(char *args[], t_map *env);

// exit.c
int
ms_exit(char *args[], t_shell *shell);

// --- Utils ---

// debug_utils.c
void
print_char_array(const char *name, char **array);

void
print_tokens(const t_token token);

void
print_cmd_table(const t_cmd_table *cmd_table);

// env_utils
t_map
create_env(char *envp[]);

void
free_envp(char ***envp_ptr);

char
**create_envp(t_map env);

// free_utils.c
void
free_str_array(char ***array_ptr);

void
free_cmd_table(t_cmd_table **cmd_table_ptr);

void
free_tokens(t_token **token_list);

// prompt_utils.c
char
*get_prompt(const t_shell *shell);

// quotes_utils.c
void
handle_quotes(char c, t_status *quotes);

char
*remove_closed_quotes(char *str);

#endif

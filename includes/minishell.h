/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:29:31 by llage             #+#    #+#             */
/*   Updated: 2025/08/30 16:26:23 by llage            ###   ########.fr       */
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

# ifndef DEBUG
#  define DEBUG false
# endif

# define SHELL_NAME "Eepyshell"
# define ANSI_RED "\001\033[31m\002"
# define ANSI_GREEN "\001\033[32m\002"
# define ANSI_YELLOW "\001\033[33m\002"
# define ANSI_BLUE "\001\033[34m\002"
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
	bool	heredoc;
}	t_cmd;

typedef struct s_shell
{
	uint8_t	exit_status;
	t_map	env;
}	t_shell;

// --- Lexing ---

// lexer.c
t_token			*lexer(char *input, t_shell shell);

// type_lexer.c
void			type_pipe(t_lexer *lexer);
void			type_exp(t_lexer *lexer);
void			type_word(t_lexer *lexer);
void			type_redir(t_lexer *lexer);
void			link_token_back(t_token *last_token, t_token *new_token);
t_token_type	find_type(t_lexer *lexer);

// --- Parsing ---

// parser.c
const t_cmd		*parser(t_token **token_list, t_shell *shell);
bool			expand_tokens(t_token *token, t_shell shell);
char			*expand_line(char *line, t_shell shell);
void			handle_quotes(char c, t_status *quotes);
bool			is_possible_char(char c, int i);
int				parse_heredoc(t_token token, bool last, t_shell shell);
bool			parse_redirs(t_cmd *cmd, t_token **token, t_shell shell);
bool			check_last(t_cmd *cmd);
bool			parse_words(t_cmd *cmd, t_token **token);
int				count_pipes(t_token **token_list);
bool			pipe_my_line(t_cmd *cmd_table, int cmd_count);
void			copy_value_to_newline(char *new_line, char *value, int *newline_iter, int *value_iter);
void			remove_quotes(char *new_line);

// --- Executing ---

// path.c
char			*find_in_path(t_map env, char *name);

// exec.c
void			exec_table(t_cmd *cmd_table, t_shell *shell);

// echo.c
int				echo(char *args[]);

// pwd.c
int				pwd(char *args[]);

// --- Utils ---

// env.c
t_map			create_env(char *envp[]);
void			free_envp(char ***envp_ptr);
char			**create_envp(t_map env);

// debug_utils.c
void			print_char_array(const char *name, char **array);
void			print_tokens(const t_token token);
void			print_cmd_table(const t_cmd *cmd_table);

// free_utils.c
void			free_str_array(char ***array_ptr);
void			free_cmd_table(t_cmd **cmd_table_ptr);
void			free_tokens(t_token **token_list);

#endif

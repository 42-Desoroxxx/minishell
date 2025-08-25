/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:29:31 by llage             #+#    #+#             */
/*   Updated: 2025/08/25 09:24:51 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <libft.h>
# include <ft_printf.h>
# include <get_next_line.h>
# include <signal.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <linux/limits.h>
# include <fcntl.h>
# include <ft_data.h>

# define DEBUG true
# define SHELL_NAME "Eepyshell"
# define ANSI_RED "\001\033[31m\002"
# define ANSI_GREEN "\001\033[32m\002"
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

// --- Lexing ---

// lexer.c
t_token			*lexer(char *input, const t_map env);

// type_lexer.c
void			type_pipe(t_lexer *lexer);
void			type_exp(t_lexer *lexer);
void			type_word(t_lexer *lexer);
void			type_redir(t_lexer *lexer);
void			link_token_back(t_token *last_token, t_token *new_token);
t_token_type	find_type(t_lexer *lexer);

// --- Parsing ---

// parser.c
const t_cmd		*parser(t_token **token_list, const t_map env);
bool			expand_tokens(t_token *token, const t_map env);
char			*expand_line(char *line, const t_map env);
void			handle_quotes(char c, t_status *quotes);
bool			is_possible_char(char c, int i);
int				parse_heredoc(t_token token, bool last, const t_map env);
bool			parse_redirs(t_cmd *cmd, t_token **token, const t_map env);
bool			check_last(t_cmd *cmd);
bool			parse_words(t_cmd *cmd, t_token **token);
int				count_pipes(t_token **token_list);

// --- Executing ---

// path.c
char			*find_in_path(t_map env, char *name);

// --- Utils ---

// env.c
t_map			create_env(char *envp[]);

// debug_utils.c
void			print_tokens(const t_token token);
void			print_cmd_table(const t_cmd *cmd_table);

// free_utils.c
void			free_str_array(char ***array_ptr);
void			free_cmd_table(t_cmd **cmd_table_ptr);
void			free_tokens(t_token **token_list);

#endif

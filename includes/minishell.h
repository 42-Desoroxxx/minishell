/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:29:31 by llage             #+#    #+#             */
/*   Updated: 2025/07/24 21:54:30 by rvitiell         ###   ########.fr       */
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

typedef enum e_token_type
{
	EMPTY,
	WORD,
	REDIR,
	EXP,
	PIPE,
}	t_token_type;

typedef enum e_status
{
	NONE,
	QUOTE,
	DOUBLE,
}	t_status;

typedef struct s_strview
{
	const char	*str;
	int			len;
}	t_strview;

typedef struct s_lexer
{
	char		*cursor;
	t_status	status;
	t_strview	line;
}	t_lexer;

typedef struct s_token
{
	struct s_token	*prev;
	struct s_token	*next;
	char			*value;
	int				token_type;
}	t_token;

int		lexer(char *input);
char    get_char_cursor(t_lexer *lexer);
char    get_next_char_cursor(t_lexer *lexer);
void    cursor_advance(t_lexer *lexer, int len);
void    skip_space(t_lexer *lexer);
void	lexer_status(t_lexer *lexer);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:29:31 by llage             #+#    #+#             */
/*   Updated: 2025/07/18 18:03:22 by rvitiell         ###   ########.fr       */
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

enum	e_token_type
{
	EMPTY = 0,
	WORD = 1,
	REDIR = 2,
	EXP = 3,
	STRING = 4,
};

typedef struct s_token
{
	struct s_token	*prev;
	struct s_token	*next;
	struct s_token	*string;
	char			*value;
	int				token_type;
}	t_token;

int	lexer(char *input);

#endif

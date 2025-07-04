/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:29:31 by llage             #+#    #+#             */
/*   Updated: 2025/05/30 20:29:31 by llage            ###   ########.fr       */
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
#include <linux/limits.h>

enum	token_type
{
	WORD = 1;
	EMPTY = 0;
	TAB = '\t';
	SPACE = ' ';
	QOUTE = '\'';
	LESSER = '<';
	GREATER = '>';
	NEWLINE = '\n';
	D_QUOTE = '\"';
	AMPERSAND = '&';
	SEMICOLON = ';';
	BACKSLASH = '\\';
};

typedef	struct s_token
{
	t_token	prev;
	t_token	next;
	char	*value;
	int		token_type;
}	t_token;

#endif

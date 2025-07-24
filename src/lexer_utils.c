/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 20:39:14 by rvitiell          #+#    #+#             */
/*   Updated: 2025/07/24 21:52:55 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	get_char_cursor(t_lexer *lexer)
{
	return (lexer->cursor[0]);
}

char	get_next_char_cursor(t_lexer *lexer)
{
	if (lexer->cursor[0])
		return (lexer->cursor[1]);
	return (0);
}

void	cursor_advance(t_lexer *lexer, int len)
{
	lexer->cursor += len;
}

void	skip_space(t_lexer *lexer)
{
	while (get_char_cursor(lexer) == ' ' || get_char_cursor(lexer) == '\t')
		cursor_advance(lexer, 1);
}

void	lexer_status(t_lexer *lexer)
{
	if (get_char_cursor(lexer) == '\'')
	{
		if (lexer->status == NONE)
			lexer->status = QUOTE;
		else if (lexer->status == QUOTE)
			lexer->status = NONE;
	}
	if (get_char_cursor(lexer) == '"')
	{
		if (lexer->status == NONE)
			lexer->status = DOUBLE;
		else if (lexer->status == DOUBLE)
			lexer->status = NONE;
	}
}

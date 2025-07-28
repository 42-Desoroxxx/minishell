/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:27:47 by rvitiell          #+#    #+#             */
/*   Updated: 2025/07/28 19:48:23 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	type_pipe(t_lexer *lexer)
{
	lexer->line.str = lexer->cursor;
	lexer->line.len = 1;
	cursor_advance(lexer, 1);
}

void	type_exp(t_lexer *lexer)
{
	char	*save;
	bool	strfirst;

	save = lexer->cursor;
	strfirst = true;
	cursor_advance(lexer, 1);
	while (true)
	{
		if (strfirst)
		{
			strfirst = false;
			if (!(get_char_cursor(lexer, 0) == '?'
					|| get_char_cursor(lexer, 0) == '_'
					|| ft_isalpha(get_char_cursor(lexer, 0))))
				break ;
		}
		else if (!(ft_isalnum(get_char_cursor(lexer, 0))
				|| get_char_cursor(lexer, 0) == '_'))
			break ;
		cursor_advance(lexer, 1);
	}
	lexer->line.str = save;
	lexer->line.len = lexer->cursor - save;
}

void	type_word(t_lexer *lexer)
{
	char	*save;

	save = lexer->cursor;
	while (true)
	{
		lexer_status(lexer);
		if (!get_char_cursor(lexer, 0) || (get_char_cursor(lexer, 0) == ' '
				&& lexer->status == NONE))
			break ;
		cursor_advance(lexer, 1);
	}
	lexer->line.str = save;
	lexer->line.len = lexer->cursor - save;
}

void	type_redir(t_lexer *lexer)
{
	lexer->line.len = 1;
	if (get_char_cursor(lexer, 1) == '<'
		|| get_char_cursor(lexer, 1) == '>')
		lexer->line.len++;
	lexer->line.str = lexer->cursor;
	cursor_advance(lexer, lexer->line.len);
}

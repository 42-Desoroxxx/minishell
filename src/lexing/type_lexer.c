/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <rvitiell@student.42angouleme.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:27:47 by rvitiell          #+#    #+#             */
/*   Updated: 2025/07/28 19:48:23 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	lexer_status(t_lexer *lexer)
{
	const char	c = lexer->line[lexer->cursor + lexer->offset];

	if (c == '\'')
	{
		if (lexer->status == NONE)
			lexer->status = QUOTE;
		else if (lexer->status == QUOTE)
			lexer->status = NONE;
	}
	if (c == '"')
	{
		if (lexer->status == NONE)
			lexer->status = DOUBLE;
		else if (lexer->status == DOUBLE)
			lexer->status = NONE;
	}
}

void	type_word(t_lexer *lexer)
{
	char	c;

	while (true)
	{
		lexer_status(lexer);
		c = lexer->line[lexer->cursor + lexer->offset];
		if (!c || (c == ' ' && lexer->status == NONE))
			break ;
		lexer->offset++;
	}
}

void	type_redir(t_lexer *lexer)
{
	const char	c = lexer->line[lexer->cursor + lexer->offset];

	if (c == '<' || c == '>')
		lexer->offset++;
}

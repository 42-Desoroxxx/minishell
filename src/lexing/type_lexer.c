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
		if (!c || ((c == ' ' || c == '|' || c == '<' || c == '>')
				&& lexer->status == NONE))
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

void	link_token_back(t_token *last_token, t_token *new_token)
{
	while (last_token->next)
		last_token = last_token->next;
	last_token->next = new_token;
	new_token->prev = last_token;
}

// that's a lot of ifs you nasty bitch - Luna Mira Lage (Desoroxxx) 2025-08-08
t_token_type	find_type(t_lexer *lexer)
{
	const char	c = lexer->line[lexer->cursor];

	if (c == '|')
	{
		lexer->offset = 1;
		return (PIPE);
	}
	if (c == '<' || c == '>')
	{
		lexer->offset = 1;
		type_redir(lexer);
		return (REDIR);
	}
	if (c)
	{
		type_word(lexer);
		return (WORD);
	}
	return (EMPTY);
}

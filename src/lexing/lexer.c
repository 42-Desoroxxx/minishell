/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <rvitiell@student.42angouleme.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:42:38 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/12 19:26:00 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	skip_space(t_lexer *lexer)
{
	char	c;

	c = lexer->line[lexer->cursor];
	while (c == ' ' || c == '\t')
	{
		lexer->cursor++;
		c = lexer->line[lexer->cursor];
	}
}

// refactor the hell out of this - Luna Mira Lage 2025-08-11
static void	tokenize(t_token **first_token_ptr, t_lexer *lexer, t_token_type type)
{
	t_token	*new_token;
	t_token	*last_token;

	new_token = ft_calloc(1, sizeof(t_token));
	if (new_token == NULL)
	{
		perror(SHELL_NAME);
		if (*first_token_ptr != NULL)
			free_tokens(first_token_ptr);
		return ;
	}
	if (type == WORD || type == REDIR)
	{
		new_token->value = ft_strndup(&(lexer->line[lexer->cursor]), lexer->offset);
		if (new_token->value == NULL)
		{
			perror(SHELL_NAME);
			free(new_token);
			if (*first_token_ptr != NULL)
				free_tokens(first_token_ptr);
			return ;
		}
	}
	new_token->type = type;
	if (*first_token_ptr == NULL)
	{
		*first_token_ptr = new_token;
		return ;
	}
	last_token = *first_token_ptr;
	while (last_token->next)
		last_token = last_token->next;
	last_token->next = new_token;
	new_token->prev = last_token;
}

// that's a lot of ifs you nasty bitch - Luna Mira Lage (Desoroxxx) 2025-08-08
static t_token_type	find_type(t_lexer *lexer)
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

t_token	*lexer(char *line, const t_map env)
{
	t_token			*token;
	t_lexer			lexer;
	t_token_type	type;

	token = NULL;
	lexer.line = line;
	lexer.cursor = 0;
	lexer.offset = 0;
	lexer.status = NONE;
	type = WORD;
	while (type != EMPTY)
	{
		skip_space(&lexer);
		type = find_type(&lexer);
		tokenize(&token, &lexer, type);
		lexer.cursor += lexer.offset;
		lexer.offset = 0;
	}
	expand_tokens(token, env);
	return (token);
}

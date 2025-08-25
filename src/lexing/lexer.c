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

static bool	tokenize(t_token **fst_token_ptr, t_lexer *lexer, t_token_type type)
{
	t_token	*new_token;

	new_token = ft_calloc(1, sizeof(t_token));
	if (new_token == NULL)
		return (false);
	if (type == WORD || type == REDIR)
	{
		new_token->value = ft_strndup(&(lexer->line[lexer->cursor]),
				lexer->offset);
		if (new_token->value == NULL)
		{
			free(new_token);
			return (false);
		}
	}
	new_token->type = type;
	if (*fst_token_ptr == NULL)
	{
		*fst_token_ptr = new_token;
		return (true);
	}
	link_token_back(*fst_token_ptr, new_token);
	return (true);
}

static t_lexer	lexer_bzero(void)
{
	return ((t_lexer){
		.line = NULL,
		.cursor = 0,
		.offset = 0,
		.status = NONE
	});
}

static bool	get_type_tokenize(t_lexer *lexer,
	t_token_type *type, t_token **token)
{
	skip_space(lexer);
	*type = find_type(lexer);
	if (!tokenize(token, lexer, *type))
	{
		perror(SHELL_NAME);
		free_tokens(token);
		return (false);
	}
	lexer->cursor += lexer->offset;
	lexer->offset = 0;
	return (true);
}

t_token	*lexer(char *line, const t_map env)
{
	t_token			*token;
	t_lexer			lexer;
	t_token_type	type;

	token = NULL;
	lexer = lexer_bzero();
	lexer.line = line;
	type = WORD;
	while (type != EMPTY)
		if (!get_type_tokenize(&lexer, &type, &token))
			return (NULL);
	if (!expand_tokens(token, env))
	{
		perror(SHELL_NAME);
		free_tokens(&token);
		return (NULL);
	}
	return (token);
}

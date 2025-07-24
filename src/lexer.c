/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:42:38 by rvitiell          #+#    #+#             */
/*   Updated: 2025/07/24 21:53:45 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//get your shit together Romane
//it's essentialy a double linked list, you can do it
//well done you did it
//now you have to make it link in reverse
//that is done too, you are so good
//make the get_type work
//and create a custom split for shell commands

void	tokenize(t_token **token_list, char *word, int type)
{
	t_token	*new_token;
	t_token	*last_token;

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return ;
	new_token->value = ft_calloc(ft_strlen(word) + 1, sizeof(char));
	if (!(new_token->value))
		return ;
	ft_strlcpy(new_token->value, word, ft_strlen(word) + 1);
	if (!(*token_list))
	{
		*token_list = new_token;
		return ;
	}
	last_token = *token_list;
	while (last_token->next)
		last_token = last_token->next;
	last_token->next = new_token;
	new_token->prev = last_token;
	new_token->token_type = type;
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
			if (!(get_char_cursor(lexer) == '?'
					|| get_char_cursor(lexer) == '_'
					|| ft_isalpha(get_char_cursor(lexer))))
				break ;
		}
		else if (!(ft_isalnum(get_char_cursor(lexer))
				|| get_char_cursor(lexer) == '_'))
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
		if (!get_char_cursor(lexer) || (get_char_cursor(lexer) == ' '
				&& lexer->status == NONE))
			break ;
		cursor_advance(lexer, 1);
	}
	lexer->line.str = save;
	lexer->line.len = lexer->cursor - save;
}

t_token_type	find_type(t_lexer *lexer)
{
	char	c;

	skip_space(lexer);
	c = get_char_cursor(lexer);
	if (c == '|')
	{
		lexer->line.str = lexer->cursor;
		lexer->line.len = 1;
		cursor_advance(lexer, 1);
		return (PIPE);
	}
	else if (c == '<' || c == '>')
	{
		lexer->line.len = 1;
		if (get_next_char_cursor(lexer) == '<'
			|| get_next_char_cursor(lexer) == '>')
			lexer->line.len++;
		lexer->line.str = lexer->cursor;
		cursor_advance(lexer, lexer->line.len);
		return (REDIR);
	}
	else if (c == '$')
	{
		type_exp(lexer);
		return (EXP);
	}
	else if (c)
	{
		type_word(lexer);
		return (WORD);
	}
	return (EMPTY);
}

int	lexer(char *input)
{
	t_token	*tokens;
	t_lexer	lexer;

	tokens = NULL;
	lexer.status = NONE;
	lexer.cursor = input;
	t_token_type	type = find_type(&lexer);
	while (type != EMPTY)
	{
		printf("Type du token : %d\nToken : %.*s\n", type, lexer.line.len, lexer.line.str);
		type = find_type(&lexer);
	}
	return (0);
}

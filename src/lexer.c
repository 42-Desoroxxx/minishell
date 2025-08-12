/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <rvitiell@student.42angouleme.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:42:38 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/11 19:14:41 by rvitiell         ###   ########.fr       */
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

void	free_tokens(t_token **token_list)
{
	t_token	*tokens;

	tokens = *token_list;
	while (tokens->prev)
		tokens = tokens->prev;
	while (tokens->next)
	{
		tokens = tokens->next;
		free(tokens->prev);
	}
	free(tokens);
	*token_list = NULL;
}

// refactor the hell out of this - Luna Mira Lage 2025-08-11
void	tokenize(t_token **token_list, t_strview *strview, int type)
{
	t_token	*new_token;
	t_token	*last_token;

	new_token = ft_calloc(1, sizeof(t_token));
	if (new_token == NULL)
	{
		perror(SHELL_NAME);
		if (*token_list != NULL)
			free_tokens(token_list);
		return ;
	}
	new_token->value = strview;
	if (*token_list == NULL)
	{
		*token_list = new_token;
		new_token->token_type = type;
		return ;
	}
	last_token = *token_list;
	while (last_token->next)
		last_token = last_token->next;
	last_token->next = new_token;
	new_token->prev = last_token;
	new_token->token_type = type;
}

// that's a lot of ifs you nasty bitch - Luna Mira Lage (Desoroxxx) 2025-08-08
t_token_type	find_type(t_lexer *lexer)
{
	char	c;

	skip_space(lexer);
	c = get_char_cursor(lexer, 0);
	if (c == '|')
	{
		type_pipe(lexer);
		return (PIPE);
	}
	if (c == '<' || c == '>')
	{
		type_redir(lexer);
		return (REDIR);
	}
	if (c == '$')
	{
		type_exp(lexer);
		return (EXP);
	}
	if (c)
	{
		type_word(lexer);
		return (WORD);
	}
	return (EMPTY);
}

t_token	*lexer(char *input)
{
	t_token			*tokens;
	t_lexer			lexer;
	t_token_type	type;

	tokens = NULL;
	lexer.status = NONE;
	lexer.cursor = input;
	type = WORD;
	while (type != EMPTY)
	{
		type = find_type(&lexer);
		tokenize(&tokens, &lexer.line, type);
	}
	return (tokens);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:42:38 by rvitiell          #+#    #+#             */
/*   Updated: 2025/07/28 19:47:58 by rvitiell         ###   ########.fr       */
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
	else if (c == '<' || c == '>')
	{
		type_redir(lexer);
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
	t_token			*tokens;
	t_lexer			lexer;
	t_token_type	type;

	tokens = NULL;
	lexer.status = NONE;
	lexer.cursor = input;
	type = find_type(&lexer);
	while (type != EMPTY)
	{
		printf("Type du token : %d\nToken : %.*s\n", type, lexer.line.len, lexer.line.str);
		type = find_type(&lexer);
	}
	return (0);
}

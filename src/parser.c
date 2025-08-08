/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 03:18:16 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/08 20:51:01 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	parse_pipe(t_token *token)
{
	if (!(token->prev))
		return ; // free and error
	if (token->prev.token_type != WORD)
		return ; // free and error
}

void	parse_word(t_token *token)
{
	// implement this you kind soul
}

void	parse_redir(t_token *token)
{
	if (!(token->next))
		return ; // free and error
}

void	parser(t_token **token_list)
{
	t_token	*token;

	token = *token_list;
	while (token->token_type != EMPTY)
	{
		if (token->token_type == WORD)
			parse_word(token);
		else if (token->token_type == REDIR)
			parse_redir(token);
		else if (token->token_type == PIPE)
			parse_pipe(token);
		token = token->next;
	}
}

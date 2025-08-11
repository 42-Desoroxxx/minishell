/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <rvitiell@student.42angouleme.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 03:18:16 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/11 19:22:35 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	parse_pipe(t_token *token, t_token **token_list)
{
	if (token->prev == NULL || token->prev->token_type != WORD)
	{
		ft_fprintf(STDERR_FILENO,
			"Syntax error near unexpected token '|'\n");
		free_tokens(token_list);
	}
}

void	parse_redir(t_token *token, t_token **token_list)
{
	if (token->next->token_type != WORD)
	{
		ft_fprintf(STDERR_FILENO,
			"Syntax error near unexpected token newline\n");
		free_tokens(token_list);
	}
}

void	parser(t_token **token_list)
{
	t_token	*token;

	token = *token_list;
	while (token->next)
	{
		if (token->token_type == REDIR)
			parse_redir(token, token_list);
		else if (token->token_type == PIPE)
			parse_pipe(token, token_list);
		if (*token_list == NULL)
			break ;
		token = token->next;
	}
}

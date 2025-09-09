/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 03:05:13 by llage             #+#    #+#             */
/*   Updated: 2025/09/08 03:05:13 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	delete_token(t_token **token_ptr, t_token *token)
{
	if (token->next != NULL)
		token->next->prev = token->prev;
	if (token->prev != NULL)
		token->prev->next = token->next;
	else
		*token_ptr = token->next;
	free(token->value);
	free(token);
}

static bool	remove_quotes_from_token(t_token *token)
{
	char	*tmp;

	tmp = remove_closed_quotes(token->value);
	if (tmp == NULL)
		return (false);
	free(token->value);
	token->value = tmp;
	return (true);
}

static bool	should_expand_token(t_token *token)
{
	return (token->type == WORD
		&& (token->prev == NULL || token->prev->type != REDIR
			|| !ft_str_equal(token->prev->value, "<<")));
}

bool	expand_tokens(t_token **token_ptr, t_shell *shell)
{
	t_token	*token;
	char	*tmp;

	token = *token_ptr;
	while (token->type != EMPTY)
	{
		if (should_expand_token(token))
		{
			tmp = expand_str(token->value, shell);
			if (tmp == NULL)
				return (false);
			free(token->value);
			token->value = tmp;
			if (token->value[0] == '\0')
			{
				delete_token(token_ptr, token);
				token = *token_ptr;
				continue ;
			}
			if (!remove_quotes_from_token(token))
				return (false);
		}
		token = token->next;
	}
	return (true);
}

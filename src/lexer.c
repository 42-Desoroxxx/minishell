/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:42:38 by rvitiell          #+#    #+#             */
/*   Updated: 2025/07/04 15:13:09 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	init_token_data(t_token token, char *word)
{
	token->value = ft_calloc(ft_strlen(word) + 1, sizeof(char));
	strlcpy(token->value, word, ft_strlen(word));
}

t_token	tokenize(t_token *token, t_token prev, char *word)
{
	if (!token)
	{
		tokens = ft_calloc(1, sizeof(t_token) + 1);
		if (!tokens)
			return (NULL); //error and exit function
	}
	if (!prev)
	init_token_data(token);
	token->token_type = get_type(word);
}

int	lexer(char *input)
{
	int	i;
	char **tab_input;
	t_token	*tokens;
	t_token	*prev;

	i = 0;
	tab_input = ft_split(input, ' '); //free it later
	token = NULL;
	prev = NULL;
	while (tab_input[i])
	{
		tokens = tokenize(tokens, prev, tab_input[i]);
		tokens = prev;
		tokens = tokens->next;
		i++;
	}
	while (tokens->prev)
		tokens = tokens->prev;
}

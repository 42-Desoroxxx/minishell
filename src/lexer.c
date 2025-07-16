/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:42:38 by rvitiell          #+#    #+#             */
/*   Updated: 2025/07/10 19:31:15 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//get your shit together Romane, it's essentialy a double linked list, you can do it
//well done you did it
//now you have to make it link in reverse
//that is done too, you are so good
//make the get_type work
//and create a custom split for shell commands

char	**MS_split(char *input)
{
	
}

int	get_type(char *word)
{
	if (strlen)
}

void	tokenize(t_token **token_list, char *word)
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
}

int	lexer(char *input)
{
	int	i;
	char **tab_input;
	t_token	*tokens;

	tab_input = MS_split(input);
	i = 0;
	tokens = NULL;
	while (tab_input[i])
	{
		tokenize(&tokens, tab_input[i]);
		free(tab_input[i]);
		i++;
	}
	free(tab_input);
	return (0);
}

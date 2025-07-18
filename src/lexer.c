/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:42:38 by rvitiell          #+#    #+#             */
/*   Updated: 2025/07/18 18:01:30 by rvitiell         ###   ########.fr       */
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

int	analyse(char *result, t_token **token_list)
{
	if (strncmp(result, "echo ", 5))
	{
		tokenize(token_list, result, WORD);
		return (5);
	}
	return (0);
}

void	ft_shrink(char *src, int n)
{
	int	i;

	i = 0;
	while (src[i + n])
	{
		src[i] = src[i + n];
		i++;
	}
	while (src[i])
		src[i++] = '\0';
}

void	ms_split(char *input, t_token **token_list)
{
	char	*result;
	int		i;
	int		type;

	type = 0;
	while (input)
	{
		i = 0;
		result = ft_calloc(ft_strlen(input) + 1, sizeof(char));
		if (!result)
			return ;
		while (input[i])
		{
			result[i] = input[i];
			if (analyse(result, token_list))
			{
				ft_shrink(input, i);
				break ;
			}
			i++;
		}
		free (result);
	}
}

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

int	lexer(char *input)
{
	int		i;
	char	**tab_input;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	ms_split(input, &tokens)
	return (0);
}

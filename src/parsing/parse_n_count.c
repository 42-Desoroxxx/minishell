/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_n_count.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:16:19 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/25 07:16:21 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	count_pipes(t_token **token_list)
{
	int		count;
	t_token	*token;

	count = 0;
	token = *token_list;
	while (token)
	{
		if (token->type == PIPE)
			count++;
		token = token->next;
	}
	return (count);
}

static int	count_args(t_token *token)
{
	int	count;

	count = 0;
	while (token->type != PIPE && token->type != EMPTY)
	{
		if (token->type == REDIR)
		{
			token = token->next->next;
			continue ;
		}
		token = token->next;
		count++;
	}
	return (count);
}

bool	parse_words(t_cmd *cmd, t_token **token_ptr)
{
	int	i;

	cmd->args = ft_calloc(sizeof(char *), count_args(*token_ptr) + 1);
	if (cmd->args == NULL)
		return (false);
	i = 0;
	while ((*token_ptr)->type != PIPE && (*token_ptr)->type != EMPTY)
	{
		if ((*token_ptr)->type == REDIR)
		{
			*token_ptr = (*token_ptr)->next->next;
			continue ;
		}
		cmd->args[i] = ft_strdup((*token_ptr)->value);
		if (cmd->args[i] == NULL)
			return (false);
		*token_ptr = (*token_ptr)->next;
		i++;
	}
	return (true);
}

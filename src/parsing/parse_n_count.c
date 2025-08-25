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

static int	count(t_token *token, t_token_type type)
{
	int	count;

	count = 0;
	while (token->type == type)
	{
		token = token->next;
		count++;
	}
	return (count);
}

bool	parse_words(t_cmd *cmd, t_token **token)
{
	int	i;

	cmd->args = ft_calloc(sizeof(char *), count(*token, WORD) + 1);
	if (cmd->args == NULL)
		return (false);
	i = 0;
	while ((*token)->type == WORD)
	{
		cmd->args[i] = ft_strdup((*token)->value);
		if (cmd->args[i] == NULL)
			return (false);
		*token = (*token)->next;
		i++;
	}
	return (true);
}

bool	check_last(t_cmd *cmd)
{
	if (cmd->in_redir == -2 || cmd->out_redir == -2)
	{
		ft_fprintf(STDERR_FILENO,
			SHELL_NAME " [Error] :  Somehow there was no last...\n");
		return (false);
	}
	return (true);
}

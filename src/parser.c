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

#include "libft.h"
#include <minishell.h>
#include <time.h>

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

int	count_pipes(t_token **token_list)
{
	int		count;
	t_token	*token;

	count = 0;
	token = *token_list;
	while (token)
	{
		if (token->token_type == PIPE)
			count++;
		token = token->next;
	}
	return (count);
}

const t_cmd	*build_cmd_table(t_token **token_list)
{
	const t_cmd	*cmd_table = ft_calloc(count_pipes(token_list) + 1,
			sizeof(t_cmd));

	if (cmd_table == NULL)
	{
		perror(SHELL_NAME);
		free_tokens(token_list);
	}
	return (cmd_table);
}

const t_cmd	*parser(t_token **token_list)
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
	return (build_cmd_table(token_list));
}

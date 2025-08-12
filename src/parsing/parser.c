/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <rvitiell@student.42angouleme.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 03:18:16 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/12 19:27:00 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <minishell.h>
#include <time.h>

static void	parse_pipe(t_token *token, t_token **token_list)
{
	if (token->prev == NULL || token->prev->type != WORD)
	{
		ft_fprintf(STDERR_FILENO,
			"Syntax error near unexpected token '|'\n");
		free_tokens(token_list);
	}
}

static void	parse_redir(t_token *token, t_token **token_list)
{
	if (token->next->type != WORD)
	{
		ft_fprintf(STDERR_FILENO,
			"Syntax error near unexpected token newline\n");
		free_tokens(token_list);
	}
}

static int	count_pipes(t_token **token_list)
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

static bool	parse_words(t_cmd *cmd, t_token **token)
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

static bool parse_redirs(t_cmd *cmd, t_token **token)
{
	int	i;


	return (true);
}

const t_cmd	*build_cmd_table(t_token **token_list)
{
	const int	cmd_count = count_pipes(token_list) + 1;
	t_cmd		*cmd_table;
	t_token		*token;
	int			i;

	cmd_table = ft_calloc(cmd_count + 1, sizeof(t_cmd));
	if (cmd_table == NULL)
	{
		perror(SHELL_NAME);
		free_tokens(token_list);
	}
	i = 0;
	token = *token_list;
	while (i < cmd_count)
	{
		if (token->type == PIPE)
		{
			token = token->next;
			continue ;
		}
		if (!parse_words(&cmd_table[i], &token) && !parse_redirs(&cmd_table[i], &token))
		{
			perror(SHELL_NAME);
			free_tokens(token_list);
			free_cmd_table(&cmd_table);
			return (NULL);
		}
		if (token->type == EMPTY)
			break ;
		i++;
	}
	return (cmd_table);
}

const t_cmd	*parser(t_token **token_list)
{
	t_token	*token;

	token = *token_list;
	while (token->next)
	{
		if (token->type == REDIR)
			parse_redir(token, token_list);
		else if (token->type == PIPE)
			parse_pipe(token, token_list);
		if (*token_list == NULL)
			return (NULL);
		token = token->next;
	}
	return (build_cmd_table(token_list));
}

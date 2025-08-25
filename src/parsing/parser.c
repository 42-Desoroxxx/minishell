/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <rvitiell@student.42angouleme.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 03:18:16 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/14 16:44:11 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	check_pipe(t_token *token, t_token **token_list)
{
	if (token->next->type == EMPTY
		|| token->prev == NULL || token->prev->type != WORD)
	{
		ft_fprintf(STDERR_FILENO,
			"Syntax error near unexpected token '|'\n");
		free_tokens(token_list);
	}
}

static void	check_redir(t_token *token, t_token **token_list)
{
	if (token->next->type != WORD)
	{
		ft_fprintf(STDERR_FILENO,
			"Syntax error near unexpected token newline\n");
		free_tokens(token_list);
	}
}

static bool	build_cmd_line(t_token **token_ptr, const t_map env,
		t_cmd *cmd_table, int i)
{
	if ((*token_ptr)->type == PIPE)
		*token_ptr = (*token_ptr)->next;
	if (!parse_words(&cmd_table[i], token_ptr)
		|| !parse_redirs(&cmd_table[i], token_ptr, env))
	{
		perror(SHELL_NAME);
		free_tokens(token_ptr);
		free_cmd_table(&cmd_table);
		return (false);
	}
	return (true);
}

static const t_cmd	*build_cmd_table(t_token **token_ptr, const t_map env)
{
	const int	cmd_count = count_pipes(token_ptr) + 1;
	t_cmd		*cmd_table;
	int			i;

	cmd_table = ft_calloc(cmd_count + 1, sizeof(t_cmd));
	if (cmd_table == NULL)
	{
		perror(SHELL_NAME);
		free_tokens(token_ptr);
		return (NULL);
	}
	i = -1;
	while (++i < cmd_count)
	{
		if (!build_cmd_line(token_ptr, env, cmd_table, i))
			return (NULL);
		if ((*token_ptr)->type == EMPTY)
			break ;
	}
	return (cmd_table);
}

const t_cmd	*parser(t_token **token_list, const t_map env)
{
	t_token	*token;

	token = *token_list;
	while (token->next)
	{
		if (token->type == REDIR)
			check_redir(token, token_list);
		else if (token->type == PIPE)
			check_pipe(token, token_list);
		if (*token_list == NULL)
			return (NULL);
		token = token->next;
	}
	return (build_cmd_table(token_list, env));
}

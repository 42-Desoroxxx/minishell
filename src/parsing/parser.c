/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <rvitiell@student.42angouleme.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 03:18:16 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/30 23:11:49 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	check_word(t_token *token, t_token **token_list)
{
	t_status	quotes;
	int			single_quotes;
	int			double_quotes;
	int			i;

	i = -1;
	single_quotes = 0;
	double_quotes = 0;
	quotes = NONE;
	while (token->value[++i])
	{
		if (is_quote(token->value[i]))
			handle_quotes(token->value[i], &quotes);
		if (token->value[i] == '\'' && quotes != DOUBLE)
			single_quotes++;
		else if (token->value[i] == '"' && quotes != QUOTE)
			double_quotes++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
	{
		ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
			" [Error]: Syntax error, unclosed quotes\n" ANSI_RESET);
		free_tokens(token_list);
	}
}

static void	check_redir(t_token *token, t_token **token_list)
{
	if (token->next->type != WORD)
	{
		ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
			" [Error]: Syntax error near unexpected token newline\n" ANSI_RESET);
		free_tokens(token_list);
	}
}

static void	check_pipe(t_token *token, t_token **token_list)
{
	if (token->prev == NULL || token->prev->type != WORD
		|| token->next->type == EMPTY)
	{
		ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
			" [Error]: Syntax error near unexpected token '|'\n" ANSI_RESET);
		free_tokens(token_list);
	}
}

static bool	build_cmd_line(t_token **token_ptr, t_shell *shell, t_cmd *cmd)
{
	if ((*token_ptr)->type == PIPE)
		*token_ptr = (*token_ptr)->next;
	if ((*token_ptr)->type == REDIR)
	{
		if (!parse_redirs(cmd, token_ptr, shell) || !parse_words(cmd, token_ptr))
			return (false);
	}
	else
		if (!parse_words(cmd, token_ptr) || !parse_redirs(cmd, token_ptr, shell))
			return (false);
	return (true);
}

static t_cmd_table	*build_cmd_table(t_token **token_ptr, t_shell *shell)
{
	const int	cmd_count = count_pipes(token_ptr) + 1;
	t_cmd_table	*cmd_table;
	int			i;

	cmd_table = ft_calloc(1, sizeof(t_cmd_table) + (cmd_count * sizeof(t_cmd)));
	if (cmd_table == NULL)
		return (NULL);
	cmd_table->size = cmd_count;
	i = -1;
	while (++i < cmd_count)
	{
		if (!build_cmd_line(token_ptr, shell, &cmd_table->cmds[i]))
			return (NULL);
		if ((*token_ptr)->type == EMPTY)
			break ;
	}
	if (!pipe_my_line(cmd_table))
		return (NULL);
	return (cmd_table);
}

t_cmd_table	*parser(t_token **token_list, t_shell *shell)
{
	t_token	*token;

	token = *token_list;
	while (token->next)
	{
		if (token->type == WORD)
			check_word(token, token_list);
		else if (token->type == REDIR)
			check_redir(token, token_list);
		else if (token->type == PIPE)
			check_pipe(token, token_list);
		if (*token_list == NULL)
		{
			shell->exit_status = 2;
			return (NULL);
		}
		token = token->next;
	}
	if (!expand_tokens(token_list, shell))
	{
		perror(SHELL_NAME);
		free_tokens(token_list);
		return (NULL);
	}
	if (DEBUG)
		print_tokens(**token_list);
	return (build_cmd_table(token_list, shell));
}

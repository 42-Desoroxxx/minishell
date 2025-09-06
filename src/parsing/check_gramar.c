/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_gramar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 21:44:00 by rvitiell          #+#    #+#             */
/*   Updated: 2025/09/06 21:44:02 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	check_word(t_token *token, t_token **token_list)
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

void	check_redir(t_token *token, t_token **token_list)
{
	if (token->next->type == EMPTY)
	{
		ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
			" [Error]: Syntax error near unexpected token newline\n"
			ANSI_RESET);
		free_tokens(token_list);
	}
	else if (token->next->type == PIPE)
	{
		ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
			" [Error]: Syntax error near unexpected token '|'\n"
			ANSI_RESET);
		free_tokens(token_list);
	}
	else if (token->next->type == REDIR)
	{
		ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
			" [Error]: Syntax error near unexpected token '%s'\n"
			ANSI_RESET, token->next->value);
		free_tokens(token_list);
	}
}

void	check_pipe(t_token *token, t_token **token_list)
{
	if (token->prev == NULL || token->prev->type != WORD
		|| token->next->type == EMPTY)
	{
		ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
			" [Error]: Syntax error near unexpected token '|'\n" ANSI_RESET);
		free_tokens(token_list);
	}
}

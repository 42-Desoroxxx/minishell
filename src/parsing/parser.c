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

static bool	build_cmd_line(t_token **token_ptr, t_shell *shell, t_cmd *cmd)
{
	if ((*token_ptr)->type == PIPE)
		*token_ptr = (*token_ptr)->next;
	if ((*token_ptr)->type == REDIR)
	{
		if (!parse_redirs(cmd, token_ptr, shell)
			|| !parse_words(cmd, token_ptr))
			return (false);
	}
	else
		if (!parse_words(cmd, token_ptr)
			|| !parse_redirs(cmd, token_ptr, shell))
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

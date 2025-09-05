/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:11:27 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/25 07:11:29 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	count_redirs(t_token *token, int *in, int *out)
{
	int	total;

	total = 0;
	*in = 0;
	*out = 0;
	while (token != NULL && token->type != PIPE && token->type != EMPTY)
	{
		if (token->type == REDIR)
		{
			if (ft_strncmp(token->value, "<", 1) == 0)
				(*in)++;
			if (ft_strncmp(token->value, ">", 1) == 0)
				(*out)++;
			total++;
		}
		token = token->next->next;
	}
	return (total);
}

static int	parse_redir(t_token *token, const bool last)
{
	if (access(token->value, F_OK) == -1)
		return (-1);
	if (last)
		return (open(token->value, O_RDONLY | O_CLOEXEC));
	return (-2);
}

static int	parse_overwrite(t_token *token, const bool last)
{
	int	fd;

	fd = open(token->value, O_CREAT | O_WRONLY | O_CLOEXEC | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	if (last)
		return (fd);
	close(fd);
	return (-2);
}

static int	parse_append(t_token *token, const bool last)
{
	int	fd;

	fd = open(token->value, O_CREAT | O_WRONLY | O_CLOEXEC | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
	if (last)
		return (fd);
	close(fd);
	return (-2);
}

bool	parse_redirs(t_cmd *cmd, t_token **token_ptr, t_shell *shell)
{
	int	total;
	int	in_max;
	int	out_max;
	int	in;
	int	out;

	total = count_redirs(*token_ptr, &in_max, &out_max);
	in = 0;
	out = 0;
	while (total != 0)
	{
		if (ft_strncmp((*token_ptr)->value, "<", 2) == 0)
			cmd->in_redir = parse_redir((*token_ptr)->next, ++in == in_max);
		else if (ft_strncmp((*token_ptr)->value, "<<", 3) == 0)
			cmd->in_redir = parse_heredoc((*token_ptr)->next, ++in == in_max, shell);
		else if (ft_strncmp((*token_ptr)->value, ">", 2) == 0)
			cmd->out_redir = parse_overwrite((*token_ptr)->next, ++out == out_max);
		else if (ft_strncmp((*token_ptr)->value, ">>", 3) == 0)
			cmd->out_redir = parse_append((*token_ptr)->next, ++out == out_max);
		*token_ptr = (*token_ptr)->next->next;
		if (cmd->in_redir == -1 || cmd->out_redir == -1)
			shell->exit_status = 1;
		if (cmd->in_redir == -1 || cmd->out_redir == -1)
			return (false);
		total--;
	}
	return (check_last(cmd));
}

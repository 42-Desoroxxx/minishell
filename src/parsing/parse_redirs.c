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
	int	i;

	i = 0;
	*in = 0;
	*out = 0;
	while (token->type != PIPE && token->type != EMPTY)
	{
		if (token->type == REDIR)
		{
			if (ft_str_equal(token->value, "<")
				|| ft_str_equal(token->value, "<<"))
				(*in)++;
			if (ft_str_equal(token->value, ">")
				|| ft_str_equal(token->value, ">>"))
				(*out)++;
			i++;
		}
		token = token->next->next;
	}
	return (i);
}

static int	parse_redir(t_token token, bool last)
{
	int	fd;

	if (access(token.value, F_OK) == -1)
		return (-1);
	if (last)
	{
		fd = open(token.value, O_RDONLY);
		return (fd);
	}
	return (-2);
}

static int	parse_overwrite(t_token token, bool last)
{
	int	fd;

	fd = open(token.value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (last)
		return (fd);
	fd = close(fd);
	if (fd == 0)
		return (-2);
	return (fd);
}

static int	parse_append(t_token token, bool last)
{
	int	fd;

	fd = open(token.value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (last)
		return (fd);
	fd = close(fd);
	if (fd == 0)
		return (-2);
	return (fd);
}

bool	parse_redirs(t_cmd *cmd, t_token **token, t_shell shell)
{
	int	in;
	int	out;
	int	in_max;
	int	out_max;
	int	count;

	count = count_redirs(*token, &in_max, &out_max);
	in = 0;
	out = 0;
	while (count != 0)
	{
		if (ft_strncmp((*token)->value, "<", 2) == 0)
			cmd->in_redir = parse_redir(*(*token)->next, ++in == in_max);
		else if (ft_strncmp((*token)->value, "<<", 3) == 0)
			cmd->in_redir = parse_heredoc(*(*token)->next, ++in == in_max, shell);
		else if (ft_strncmp((*token)->value, ">", 2) == 0)
			cmd->out_redir = parse_overwrite(*(*token)->next, ++out == out_max);
		else if (ft_strncmp((*token)->value, ">>", 3) == 0)
			cmd->out_redir = parse_append(*(*token)->next, ++out == out_max);
		(*token) = (*token)->next->next;
		if (cmd->in_redir == -1 || cmd->out_redir == -1)
			return (false);
		count--;
	}
	return (check_last(cmd));
}

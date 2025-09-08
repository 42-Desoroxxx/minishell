/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:11:27 by rvitiell          #+#    #+#             */
/*   Updated: 2025/09/05 11:56:33 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
			token = token->next;
		}
		token = token->next;
	}
	return (total);
}

bool	parse_redirs(t_cmd *cmd, t_token **token_ptr, t_token *token,
	t_shell *shell)
{
	int		total;
	int		max[2];
	int		in;
	int		out;

	total = count_redirs(*token_ptr, &max[0], &max[1]) + 1;
	in = 0;
	out = 0;
	while (--total != 0)
	{
		while (token->type != REDIR)
			token = token->next;
		if (ft_str_equal(token->value, "<") && cmd->in_redir != -1)
			cmd->in_redir = parse_redir(token->next, ++in == max[0]);
		else if (ft_str_equal(token->value, "<<"))
			cmd->in_redir = parse_heredoc(token->next, ++in == max[0], shell,
					cmd->in_redir);
		else if (ft_str_equal(token->value, ">") && cmd->out_redir != -1)
			cmd->out_redir = parse_overwrite(token->next, ++out == max[1]);
		else if (ft_str_equal(token->value, ">>") && cmd->out_redir != -1)
			cmd->out_redir = parse_append(token->next, ++out == max[1]);
		token = token->next;
	}
	return (check_last(cmd));
}

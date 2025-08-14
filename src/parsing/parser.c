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

static int	count_redirs(t_token *token, int *in, int *out)
{
	int	i;

	i = 0;
	while (token->type != PIPE)
	{
		if (token->type == REDIR)
		{
			if (ft_strncmp(token->value, "<", 1) == 0)
				(*in)++;
			if (ft_strncmp(token->value, ">", 1) == 0)
				(*out)++;
			i++;
		}
		token = token->next->next;
	}
	return (i);
}

// Check if the file exist, if not error
// If last, return an opened fd.
// Token is the target, so the file name.
static int	parse_redir(t_token token, bool last)
{
	int	fd;

	if (access(token.value, F_OK) == -1)
		return (-1);
	if (last)
	{
		fd = open(token.value, O_RDWR);
		return (fd);
	}
	return (-2);
}

// Open the fd, wait for the delimiter.
// If not last you can close and return here.
// Remove the delimiter from the file.
// If the delimiter does NOT contains quotes or single quotes expand the content.
// If last, return an opened fd.
// Token is the delimiter.
static int	parse_heredoc(t_token token, t_token delimiter, bool last)
{

}

// Check if the file exist, if not create.
// If last, return an opened fd for overwrite.
// Token is the target, so the file name.
static int	parse_overwrite(t_token token, bool last)
{
	int	fd;

	fd = open(token.value, O_RDWR, O_CREAT);
	if (last)
		return (fd);
	fd = close(fd);
	if (fd == 0)
		return (-2);
	return (fd);
}

// Check if the file exist, if not create.
// If last, return an opened fd for appending.
// Token is the target, so the file name.
static int	parse_append(t_token token, bool last)
{
	int	fd;

	fd = open(token.value, O_RDWR, O_CREAT, O_APPEND);
	if (last)
		return (fd);
	fd = close(fd);
	if (fd == 0)
		return (-2);
	return (fd);
}

static bool parse_redirs(t_cmd *cmd, t_token **token)
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
		(*token) = (*token)->next->next;

		if (ft_strncmp((*token)->value, "<", 2) == 0)
			cmd->in_redir = parse_redir(**token, in++ == in_max);
		else if (ft_strncmp((*token)->value, "<<", 3) == 0)
			cmd->in_redir = parse_heredoc(*token, in++ == in_max);
		else if (ft_strncmp((*token)->value, ">", 2) == 0)
			cmd->out_redir = parse_overwrite(**token, out++ == out_max);
		else if (ft_strncmp((*token)->value, ">>", 3) == 0)
			cmd->out_redir = parse_append(**token, out++ == out_max);

		if (cmd->in_redir == -1 || cmd->out_redir == -1)
			return (false);

		count--;
	}

	if (cmd->in_redir == -2 || cmd->out_redir == -2)
	{
		ft_fprintf(STDERR_FILENO, SHELL_NAME ": [Error] Somehow there was no last...");
		return (false);
	}

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
			check_redir(token, token_list);
		else if (token->type == PIPE)
			check_pipe(token, token_list);
		if (*token_list == NULL)
			return (NULL);
		token = token->next;
	}
	return (build_cmd_table(token_list));
}

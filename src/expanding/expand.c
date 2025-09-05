/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:07:25 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/19 23:07:27 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*replace_expand_with_value(char *line, char *expand,
	char *value, int len)
{
	int		line_iter;
	int		newline_iter;
	int		value_iter;
	char	*new_line;

	line_iter = 0;
	newline_iter = 0;
	value_iter = 0;
	new_line = ft_calloc(ft_strlen(line) + ft_strlen(value)
			- len + 1, sizeof(char *));
	if (new_line == NULL)
		return (NULL);
	while (line_iter < (int)ft_strlen(line))
	{
		if (line + line_iter == expand)
		{
			copy_value(new_line, value, &newline_iter, &value_iter);
			line_iter += len;
		}
		else
			new_line[newline_iter++] = line[line_iter++];
	}
	new_line[newline_iter] = '\0';
	return (new_line);
}

static char	*expanding(char *line, char *expand, int len, const t_map env)
{
	char	*new_line;
	char	*value;

	new_line = ft_calloc(len + 1, sizeof(char *));
	if (new_line == NULL)
		return (NULL);
	ft_strlcpy(new_line, expand + 1, len);
	value = map_get(&env, new_line);
	free(new_line);
	new_line = replace_expand_with_value(line, expand, value, len);
	return (new_line);
}

static char	*isolate_expand(char *line, int *i, t_shell *shell)
{
	int		j;
	char	*new_line;
	char	*value;

	j = 1;
	if (line[(*i) + j] != '?')
	{
		while (line[(*i) + j] && is_possible_char(line[(*i) + j], j))
			j++;
		new_line = expanding(line, &line[(*i)], j, shell->env);
	}
	else
	{
		value = ft_itoa(shell->exit_status);
		if (value == NULL)
			return (NULL);
		new_line = replace_expand_with_value(line, &line[(*i)], value, 2);
		free(value);
	}
	*i += j;
	return (new_line);
}

char	*expand_line(char *line, t_shell *shell)
{
	int			i;
	t_status	quotes;
	char		*new_line;

	i = -1;
	quotes = NONE;
	new_line = NULL;
	while (++i < (int)ft_strlen(line))
	{
		if (line[i] == '\'' || line[i] == '"')
			handle_quotes(line[i], &quotes);
		if ((line[i] == '?' || line[i] == '$')
			&& quotes != QUOTE && is_possible_char(line[i + 1], 1))
		{
			if (new_line != NULL)
				ft_safe_free((void *) &new_line);
			new_line = isolate_expand(line, &i, shell);
			if (new_line == NULL)
				return (NULL);
		}
	}
	if (new_line == NULL)
		return (line);
	free(line);
	return (new_line);
}

bool	expand_tokens(t_token **token_list, t_shell *shell)
{
	char	*new_line;
	t_token	*token;
	t_token	*tmp;

	token = *token_list;
	while (token->type != EMPTY)
	{
		if (token->type != WORD || token->value[0] != '$'
			|| (token->prev != NULL && token->prev->type == REDIR && ft_str_equal(token->prev->value, "<<")))
		{
			token = token->next;
			continue ;
		}
		new_line = expand_line(token->value, shell);
		if (new_line == NULL)
			return (false);
		if (new_line[0] == '\0')
		{
			free(new_line);
			if (token->prev != NULL)
			{
				token->prev->next = token->next;
				token->next->prev = token->prev;
			}
			else
			{
				token->next->prev = NULL;
				*token_list = token->next;
			}
			tmp = token;
			token = token->next;
			free(tmp);
			continue ;
		}
		token->value = new_line;
		token = token->next;
	}
	token = *token_list;
	while (token->type != EMPTY)
	{
		if (token->type != WORD)
		{
			token = token->next;
			continue ;
		}
		new_line = remove_closed_quotes(token->value);
		if (new_line == NULL)
			return (false);
		free(token->value);
		token->value = new_line;
		token = token->next;
	}
	return (true);
}

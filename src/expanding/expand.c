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

static char	*expand(char *str, int start, t_shell *shell, t_status quotes)
{
	size_t	new_line_len;
	char	*new_line;
	int		key_len;
	char	*value;
	int		i;

	new_line = get_expand_key(str, start);
	if (new_line == NULL)
		return (NULL);
	key_len = ft_strlen(new_line);
	value = get_expand_value(new_line, quotes, shell);
	free(new_line);
	new_line_len = ft_strlen(value) + (ft_strlen(str) - key_len);
	new_line = ft_calloc(new_line_len, sizeof(char));
	if (new_line == NULL)
		return (NULL);
	if (start > 0)
		ft_strncpy(new_line, str, start);
	ft_strlcat(new_line, value, new_line_len);
	i = start + key_len + 1;
	ft_strlcat(new_line, &str[i], new_line_len);
	return (new_line);
}

char	*expand_str(char *str, t_shell *shell)
{
	t_status	quotes;
	char		*line;
	char		*tmp;
	size_t		i;

	i = -1;
	quotes = NONE;
	line = ft_strdup(str);
	while (++i < ft_strlen(line))
	{
		handle_quotes(line[i], &quotes);
		if (line[i] == '$' && quotes != QUOTE && is_valid_char(line[i + 1]))
		{
			tmp = expand(line, i, shell, quotes);
			i = -1;
			free(line);
			if (tmp == NULL)
				return (NULL);
			line = tmp;
			if (line[0] == '\0')
				break ;
		}
	}
	free(str);
	return (line);
}

static void	delete_token(t_token **token_ptr, t_token *token)
{
	if (token->next != NULL)
		token->next->prev = token->prev;
	if (token->prev != NULL)
		token->prev->next = token->next;
	else
		*token_ptr = token->next;
	free(token->value);
	free(token);
}

static bool	remove_quotes_from_token(t_token *token)
{
	char	*tmp;

	tmp = remove_closed_quotes(token->value);
	if (tmp == NULL)
		return (false);
	free(token->value);
	token->value = tmp;
	return (true);
}

bool	expand_tokens(t_token **token_ptr, t_shell *shell)
{
	t_token	*token;

	token = *token_ptr;
	while (token->type != EMPTY)
	{
		if (token->type == WORD
			&& (token->prev == NULL || token->prev->type != REDIR
				|| !(token->prev->value[0] == '<'
					&& token->prev->value[1] == '<')))
		{
			token->value = expand_str(token->value, shell);
			if (token->value == NULL)
				return (false);
			if (token->value[0] == '\0')
			{
				delete_token(token_ptr, token);
				token = *token_ptr;
				continue ;
			}
			if (!remove_quotes_from_token(token))
				return (false);
		}
		token = token->next;
	}
	return (true);
}

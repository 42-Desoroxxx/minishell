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

static char	*get_key(char *str, int start)
{
	int		key_len;
	char	*key;
	int		i;
	int		j;

	if (str[start + 1] == '?')
		return (ft_strdup("?"));
	i = start;
	key_len = 0;
	while (str[++i] && str[i] != '$' && is_possible_char(str[i]) && !is_quote(str[i]))
		key_len++;
	key = ft_calloc(key_len + 1, sizeof(char));
	if (key == NULL)
		return (NULL);
	i = start;
	j = 0;
	while (str[++i] && str[i] != '$' && is_possible_char(str[i]) && !is_quote(str[i]))
		key[j++] = str[i];
	return (key);
}

static char	*expand(char *str, int start, t_shell *shell, t_status quotes)
{
	char	*new_line;
	int		key_len;
	char	*tmp;
	int		i;

	new_line = get_key(str, start);
	if (new_line == NULL)
		return (NULL);
	key_len = ft_strlen(new_line);
	tmp = map_get(&shell->env, new_line);
	if (ft_str_equal(new_line, "?"))
		tmp = ft_byte_to_str(shell->exit_status);
	if (new_line[0] == '\0' || tmp == NULL)
		tmp = "";
	if (new_line[0] == '\0' && quotes != NONE)
		tmp = "$";
	free(new_line);
	new_line = ft_calloc(ft_strlen(tmp) + (ft_strlen(str) - key_len),
		sizeof(char));
	if (new_line == NULL)
		return (NULL);
	if (start > 0)
		ft_strncpy(new_line, str, start);
	ft_strlcat(new_line, tmp, ft_strlen(tmp) + (ft_strlen(str) - key_len));
	i = start + key_len + 1;
	ft_strlcat(new_line, &str[i], ft_strlen(tmp) + (ft_strlen(str) - key_len));
	return (new_line);
}

char	*expand_str(char *str, t_shell *shell)
{
	char		*new_line;
	char		*tmp;
	t_status	quotes;
	size_t		i;

	i = -1;
	quotes = NONE;
	new_line = ft_strdup(str);
	while (++i < ft_strlen(new_line))
	{
		if (new_line[i] == '\'' || new_line[i] == '"')
			handle_quotes(new_line[i], &quotes);
		if (new_line[i] == '$' && quotes != QUOTE
			&& is_possible_char(new_line[i + 1]))
		{
			tmp = expand(new_line, i, shell, quotes);
			i = -1;
			free(new_line);
			if (tmp == NULL)
				return (NULL);
			new_line = tmp;
			if (new_line[0] == '\0')
				break ;
		}
	}
	free(str);
	return (new_line);
}

bool	expand_tokens(t_token **token_ptr, t_shell *shell)
{
	t_token	*token;
	char	*tmp;

	token = *token_ptr;
	while (token->type != EMPTY)
	{
		if (token->type == WORD
			&& (token->prev == NULL || token->prev->type != REDIR))
		{
			token->value = expand_str(token->value, shell);
			if (token->value == NULL)
				return (false);
			if (token->value[0] == '\0')
			{
				free(token->value);
				if (token->next != NULL)
					token->next->prev = token->prev;
				if (token->prev != NULL)
					token->prev->next = token->next;
				else
					*token_ptr = token->next;
				free(token);
				token = *token_ptr;
				continue ;
			}
        }
		token = token->next;
	}
	token = *token_ptr;
	while (token->type != EMPTY)
	{
		if (token->type == WORD
			&& (token->prev == NULL || token->prev->type != REDIR))
		{
			tmp = remove_closed_quotes(token->value);
			if (tmp == NULL)
				return (false);
			free(token->value);
			token->value = tmp;
		}
		token = token->next;
	}
	return (true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:52:16 by rvitiell          #+#    #+#             */
/*   Updated: 2025/09/01 02:49:58 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*get_expand_key(char *str, int start)
{
	int		key_len;
	char	*key;
	int		i;
	int		j;

	if (str[start + 1] == '?')
		return (ft_strdup("?"));
	i = start;
	key_len = 0;
	while (str[++i] && str[i] != '$' && is_valid_char(str[i])
		&& !is_quote(str[i]))
		key_len++;
	key = ft_calloc(key_len + 1, sizeof(char));
	if (key == NULL)
		return (NULL);
	i = start;
	j = 0;
	while (str[++i] && str[i] != '$' && is_valid_char(str[i])
		&& !is_quote(str[i]))
		key[j++] = str[i];
	return (key);
}

char	*get_expand_value(char	*key, t_status quotes, t_shell *shell)
{
	char	*value;

	value = map_get(&shell->env, key);
	if (ft_str_equal(key, "?"))
		value = ft_byte_to_str(shell->exit_status);
	if (key[0] == '\0' || value == NULL)
		value = "";
	if (key[0] == '\0' && quotes != NONE)
		value = "$";
	return (value);
}

bool	is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (true);
	return (false);
}

void	handle_quotes(char c, t_status *quotes)
{
	if (!is_quote(c))
		return ;
	if (c == '"')
	{
		if (*quotes == NONE)
			*quotes = DOUBLE;
		else if (*quotes == DOUBLE)
			*quotes = NONE;
	}
	else
	{
		if (*quotes == NONE)
			*quotes = QUOTE;
		else if (*quotes == QUOTE)
			*quotes = NONE;
	}
}

bool	is_valid_char(char c)
{
	if (c == ' ' || c == '\0' || c == '/' || c == '\\')
		return (false);
	return (true);
}

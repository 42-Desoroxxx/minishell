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

static bool	is_valid_key_start(const char c)
{
	return ((c == '_')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z'));
}

static bool	is_valid_key_char(const char c)
{
	return (is_valid_key_start(c) || (c >= '0' && c <= '9'));
}

bool	should_expand(char *line, size_t i, t_status quotes)
{
	if (line[i] != '$' || quotes == QUOTE)
		return (false);
	return (line[i + 1] == '?' || is_valid_key_start(line[i + 1]));
}

static size_t	expand_key_len(const char *str)
{
	size_t	i;

	if (str == NULL || str[0] == '\0')
		return (0);
	if (str[0] == '?')
		return (1);
	if (!is_valid_key_start(str[0]))
		return (0);
	i = 1;
	while (is_valid_key_char(str[i]))
		i++;
	return (i);
}

char	*get_expand_key(const char *str, const size_t start)
{
	size_t	key_len;
	char	*key;

	key_len = expand_key_len(&str[start + 1]);
	if (key_len == 0)
		return (ft_strdup(""));
	key = ft_calloc(key_len + 1, sizeof(char));
	if (key == NULL)
		return (NULL);
	ft_strncpy(key, &str[start + 1], key_len);
	return (key);
}

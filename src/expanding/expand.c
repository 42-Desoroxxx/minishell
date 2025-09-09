/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:07:25 by rvitiell          #+#    #+#             */
/*   Updated: 2025/09/08 20:47:24 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*get_expand_value(char *key, t_shell *shell)
{
	if (ft_str_equal(key, "?"))
		return (ft_byte_to_str(shell->exit_status));
	return (map_get(&shell->env, key));
}

static char	*expand(char *str, size_t start, t_shell *shell)
{
	size_t	final_len;
	char	*new_line;
	size_t	key_len;
	char	*value;

	new_line = get_expand_key(str, start);
	if (new_line == NULL)
		return (NULL);
	key_len = ft_strlen(new_line) + 1;
	value = get_expand_value(new_line, shell);
	free(new_line);
	final_len = ft_strlen(str) - key_len + ft_strlen(value) + 1;
	new_line = ft_calloc(final_len, sizeof(char));
	if (new_line == NULL)
		return (NULL);
	if (start > 0)
		ft_strncpy(new_line, str, start);
	ft_strlcat(new_line, value, final_len);
	ft_strlcat(new_line, &str[start + key_len], final_len);
	return (new_line);
}

static int	remove_dollar_before_quote(char *line, size_t *i, t_status quotes)
{
	if (line == NULL)
		return (0);
	if (line[*i] != '$' || quotes != NONE || !is_quote(line[*i + 1]))
		return (0);
	ft_memmove(&line[*i], &line[*i + 1], ft_strlen(&line[*i + 1]) + 1);
	if (*i > 0)
		(*i)--;
	return (1);
}

static int	do_expand(char **line, size_t *i, t_status *quotes, t_shell *shell)
{
	char	*tmp;

	tmp = expand(*line, *i, shell);
	if (!tmp)
	{
		free(*line);
		return (0);
	}
	free(*line);
	*line = tmp;
	*i = 0;
	*quotes = NONE;
	return (1);
}

char	*expand_str(char *str, t_shell *shell)
{
	t_status	quotes;
	char		*line;
	size_t		i;

	quotes = NONE;
	line = ft_strdup(str);
	if (line == NULL)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (remove_dollar_before_quote(line, &i, quotes))
			continue ;
		handle_quotes(line[i], &quotes);
		if (should_expand(line, i, quotes))
		{
			if (do_expand(&line, &i, &quotes, shell))
				continue ;
			return (NULL);
		}
		i++;
	}
	return (line);
}

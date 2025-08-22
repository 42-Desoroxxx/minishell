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

#include "ft_data.h"
#include "libft.h"
#include <minishell.h>
#include <readline/chardefs.h>
#include <stdlib.h>
#include <string.h>

void	handle_quotes(char c, t_status *quotes)
{
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
	return ;
}

static bool	is_possible_char(char c, int i)
{
	if (i == 1 && ft_isdigit(c))
		return (false);
	if (c == '"' || c =='\'' || c == ' ')
		return (false);
	return (true);
}

void	replace_expand_with_value(char *new_line, char *line, char *expand, char *value, int len)
{
	int	line_iter;
	int	newline_iter;
	int	value_iter;

	line_iter = 0;
	newline_iter = 0;
	value_iter = 0;
	while (line_iter < (int)ft_strlen(line))
	{
		if (line+line_iter == expand)
		{
			while (value[value_iter])
				new_line[newline_iter++] = value[value_iter++];
			line_iter += len;
		}
		else
			new_line[newline_iter++] = line[line_iter++];
	}
	new_line[newline_iter] = '\0';
}

char	*expanding(char *line, char *expand, int len, const t_map env)
{
	char	*new_line;
	char	*value;

	new_line = ft_calloc(len + 1, sizeof(char *));
	if (new_line == NULL)
		return (NULL);
	ft_strlcpy(new_line, expand+1, len);
	value = map_get(&env, new_line);
	free(new_line);
	if (value == NULL)
		return (NULL);
	new_line = ft_calloc(ft_strlen(line) + ft_strlen(value) - len + 1, sizeof(char *));
	if (new_line == NULL)
		return (NULL);
	replace_expand_with_value(new_line, line, expand, value, len);
	return (new_line);
}

char	*expand_line(char *line, const t_map env)
{
	int			i;
	int			j;
	t_status	quotes;
	char		*new_line;

	i = 0;
	quotes = NONE;
	new_line = NULL;
	while (i < (int)ft_strlen(line))
	{
		if (line[i] == '\'' || line[i] == '"')
			handle_quotes(line[i], &quotes);
		if (line[i] == '$' && quotes != QUOTE)
		{
			j = 1;
			if (line[i+j] != '?')
				while (line[i+j] && is_possible_char(line[i+j], j))
					j++;
			new_line = expanding(line, &line[i], j, env);
			i += j;
		}
		i++;
	}
	if (new_line)
	{
		free(line);
		return (new_line);
	}
	return (line);
}

bool	expand_tokens(t_token *token, const t_map env)
{
	while (token->type != EMPTY)
	{
		if (token->type != WORD)
		{
			token = token->next;
			continue ;
		}
		token->value = expand_line(token->value, env);
		token = token->next;
	}
	return true;
}

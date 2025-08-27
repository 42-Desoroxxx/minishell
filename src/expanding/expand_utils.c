/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:52:16 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/24 20:52:18 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

bool	is_possible_char(char c, int i)
{
	if (i == 1 && ft_isdigit(c))
		return (false);
	if (c == '"' || c == '\'' || c == ' ')
		return (false);
	return (true);
}

void	copy_value_to_newline(char *new_line, char *value,
	int *newline_iter, int *value_iter)
{
	while (value[*value_iter])
		new_line[(*newline_iter)++] = value[(*value_iter)++];
}

void	remove_quotes(char *new_line)
{
	int			iter;
	int			offset;
	t_status	old_quote;
	t_status	quotes;

	iter = -1;
	quotes = NONE;
	while (new_line[iter + 1])
	{
		iter++;
		if (new_line[iter] != '\'' && new_line[iter] != '"')
			continue ;
		old_quote = quotes;
		handle_quotes(new_line[iter], &quotes);
		offset = 0;
		if (old_quote == quotes)
			continue ;
		while (new_line[iter + offset])
		{
			new_line[iter + offset] = new_line[iter + offset + 1];
			offset++;
		}
		iter--;
	}
}

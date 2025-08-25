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

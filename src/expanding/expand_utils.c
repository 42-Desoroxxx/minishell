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

bool	is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (true);
	return (false);
}

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
}

bool	is_possible_char(char c)
{
	if (c == ' ' || c == '\0' || c == '0')
		return (false);
	return (true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 04:38:36 by llage             #+#    #+#             */
/*   Updated: 2025/09/08 04:38:36 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	is_quote(const char c)
{
	return (c == '\'' || c == '"');
}

void	handle_quotes(const char c, t_status *quotes)
{
	t_status	type;

	if (!is_quote(c))
		return ;
	if (c == '"')
		type = DOUBLE;
	else
		type = QUOTE;
	if (*quotes == NONE)
		*quotes = type;
	else if (*quotes == type)
		*quotes = NONE;
}

static int	count_quotes_to_close(char *str)
{
	t_status	quotes;
	int			single_quotes;
	int			double_quotes;
	int			i;

	i = -1;
	single_quotes = 0;
	double_quotes = 0;
	quotes = NONE;
	while (str[++i])
	{
		handle_quotes(str[i], &quotes);
		if (str[i] == '\'' && quotes != DOUBLE)
			single_quotes++;
		else if (str[i] == '"' && quotes != QUOTE)
			double_quotes++;
	}
	single_quotes = single_quotes - (single_quotes % 2);
	double_quotes = double_quotes - (double_quotes % 2);
	return (single_quotes + double_quotes);
}

char	*remove_closed_quotes(char *str)
{
	t_status	quotes;
	char		*result;
	int			i;
	int			j;

	result = ft_calloc(ft_strlen(str) - count_quotes_to_close(str) + 1,
			sizeof(char));
	if (result == NULL)
		return (NULL);
	i = -1;
	j = -1;
	quotes = NONE;
	while (str[++i])
	{
		handle_quotes(str[i], &quotes);
		if ((str[i] == '\'' && quotes != DOUBLE)
			|| (str[i] == '"' && quotes != QUOTE))
			continue ;
		result[++j] = str[i];
	}
	return (result);
}

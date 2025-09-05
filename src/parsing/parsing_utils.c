/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 03:51:19 by llage             #+#    #+#             */
/*   Updated: 2025/09/05 03:51:19 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
		if (is_quote(str[i]))
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
		if (is_quote(str[i]))
			handle_quotes(str[i], &quotes);
		if ((str[i] == '\'' && quotes != DOUBLE)
			|| (str[i] == '"' && quotes != QUOTE))
			continue ;
		result[++j] = str[i];
	}
	return (result);
}

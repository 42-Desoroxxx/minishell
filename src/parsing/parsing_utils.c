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

static void	count_closed_quotes(int *single_quotes, int *double_quotes,
	char *str)
{
	int	i;

	i = -1;
	*single_quotes = 0;
	*double_quotes = 0;
	while (str[++i])
	{
		if (str[i] == '\'')
			(*single_quotes)++;
		else if (str[i] == '"')
			(*double_quotes)++;
	}
	*single_quotes = *single_quotes - (*single_quotes % 2);
	*double_quotes = *double_quotes - (*double_quotes % 2);
}

char	*remove_closed_quotes(char *str)
{
	int		single_quotes;
	int		double_quotes;
	int		i;
	int		j;
	char	*new_delimiter;

	count_closed_quotes(&single_quotes, &double_quotes, str);
	new_delimiter = ft_calloc(ft_strlen(str) - single_quotes
			- double_quotes + 1, sizeof(char));
	if (new_delimiter == NULL)
		return (NULL);
	i = -1;
	j = -1;
	while (str[++i])
	{
		if ((str[i] == '\'' && single_quotes-- > 0)
			|| (str[i] == '"' && double_quotes-- > 0))
			continue ;
		new_delimiter[++j] = str[i];
	}
	return (new_delimiter);
}

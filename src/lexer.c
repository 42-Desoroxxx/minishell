/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:42:38 by rvitiell          #+#    #+#             */
/*   Updated: 2025/06/29 22:03:00 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

int	lexer(char *input)
{
	int	i;
	char **tab_input;
	t_tok	*tokens;

	i = 0;
	tab_input = ft_split(input, ' ');
	while (tab_input[i])
	{
		tokens = tokenize(tab_input[i]);
		tokens = tokens->next;
		i++;
	}
	while (tokens->prev)
		tokens = tokens->prev;
}

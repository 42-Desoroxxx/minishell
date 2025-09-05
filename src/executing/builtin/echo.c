/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 05:26:57 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/27 05:27:01 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	should_have_newline(const char *str)
{
	int	i;

	if (str[0] != '-' || str[1] != 'n')
		return (true);
	i = 1;
	while (str[i] && str[i] == 'n')
		i++;
	if (str[i] != '\0')
		return (true);
	return (false);
}

int	echo(char *args[])
{
	bool	newline;
	int		i;

	if (args[1] == NULL)
	{
		ft_printf("\n");
		return (0);
	}
	i = 0;
	newline = should_have_newline(args[1]);
	if (!newline)
		++i;
	while (args[++i] != NULL)
	{
		ft_printf(args[i]);
		if (args[i + 1] != NULL)
			ft_printf(" ");
	}
	if (newline)
		ft_printf("\n");
	return (0);
}

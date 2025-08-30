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

bool	is_echo_flag(char *str)
{
	int	iter;

	iter = 1;
	if (str[0] != '-')
		return (false);
	while (str[iter] && str[iter] == 'n')
		iter++;
	if (str[iter] != '\0')
		return (false);
	return (true);
}

int	ms_echo(t_cmd cmd)
{
	int		word_iter;
	bool	n_flag;

	word_iter = 1;
	n_flag = false;
	if (is_echo_flag(cmd.args[1]))
	{
		word_iter++;
		n_flag = true;
	}
	while (cmd.args[word_iter] != NULL)
	{
		ft_printf(cmd.args[word_iter]);
		word_iter++;
		if (cmd.args[word_iter] != NULL)
			ft_printf(" ");
	}
	if (!n_flag)
		ft_printf("\n");
	return (0);
}

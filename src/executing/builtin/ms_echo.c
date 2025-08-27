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

int	ms_echo(t_cmd cmd)
{
	int		word_iter;
	bool	n_flag;

	word_iter = 1;
	n_flag = false;
	if (ft_str_equal(cmd.args[1], "-n"))
	{
		word_iter++;
		n_flag = true;
	}
	while (cmd.args[word_iter] != NULL)
	{
		ft_printf(cmd.args[word_iter]);
		word_iter++;
	}
	if (!n_flag)
		ft_printf("\n");
	return (0);
}

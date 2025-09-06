/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eepy_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 22:08:31 by llage             #+#    #+#             */
/*   Updated: 2025/09/06 22:08:31 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	check_number(const char *str)
{
	int	digit_count;
	int	i;

	if (str[0] == '-' || str[0] == '+')
		i = 0;
	else
		i = -1;
	digit_count = 0;
	while (str[++i] != '\0')
	{
		if (!ft_isdigit(str[i]))
			return (false);
		digit_count++;
	}
	if (digit_count > 20)
		return (false);
	return (true);
}

int	ms_exit(char *args[], t_shell *shell)
{
	long long	status;

	ft_printf("Bye!\n");
	if (args[1] == NULL)
		exit(shell->exit_status);
	if (!check_number(args[1]))
	{
		ft_fprintf(2, ANSI_RED SHELL_NAME
			" [Error]: exit: %s: numeric argument required\n" ANSI_RESET,
			args[1]);
		exit(2);
	}
	status = ft_atol(args[1]);
	exit((uint8_t) status);
}

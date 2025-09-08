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

static bool	count_digits(const char *str, int *count)
{
	int	i;

	i = -1;
	while (str[++i] != '\0')
	{
		if (!ft_isdigit(str[i]))
			return (false);
		(*count)++;
	}
	return (true);
}

static bool	check_overflow(const char *str, bool negative)
{
	const static char	*positive_max = "9223372036854775807";
	const static char	*negative_max = "9223372036854775808";

	if (negative)
		return (ft_strncmp(str, negative_max, ft_strlen(negative_max)) <= 0);
	return (ft_strncmp(str, positive_max, ft_strlen(positive_max)) <= 0);
}

static bool	check_number(const char *str)
{
	int		digit_count;
	bool	negative;
	bool	empty;
	int		i;

	i = 0;
	empty = true;
	negative = false;
	while (str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
		negative = str[i++] == '-';
	while (str[i] == '0')
	{
		empty = false;
		i++;
	}
	digit_count = 0;
	if (!count_digits(&str[i], &digit_count))
		return (false);
	if ((empty && digit_count == 0) || digit_count > 19)
		return (false);
	if (digit_count < 19)
		return (true);
	return (check_overflow(&str[i], negative));
}

int	ms_exit(char *args[], t_shell *shell)
{
	long long	status;

	ft_fprintf(STDERR_FILENO, "Bye!\n");
	if (args[1] == NULL)
		return (shell->exit_status);
	if (!check_number(args[1]))
	{
		ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
			" [Error]: exit: %s: numeric argument required\n" ANSI_RESET,
			args[1]);
		return (2);
	}
	if (args[2] != NULL)
	{
		ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
			" [Error]: exit: too many arguments\n" ANSI_RESET);
		return (1);
	}
	status = ft_atol(args[1]);
	return ((uint8_t) status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:49:40 by llage             #+#    #+#             */
/*   Updated: 2025/09/05 16:49:40 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	key_cmp(const char *key_a, const char *key_b)
{
	const size_t	key_a_len = ft_strlen(key_a);
	const size_t	key_b_len = ft_strlen(key_b);
	size_t			max_len;

	if (key_a_len > key_b_len)
		max_len = key_a_len;
	else
		max_len = key_b_len;
	return (ft_strncmp(key_a, key_b, max_len + 1));
}

void	export_print_error(const char *name, const char *value, bool usage)
{
	ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
		" [Error]: export: %s: %s\n" ANSI_RESET,
		value, name);
	if (usage)
		ft_fprintf(STDERR_FILENO, ANSI_RED
			"export: usage: export [name[=value] ...]\n" ANSI_RESET);
}

/**
 * @return true if an invalid option was found, false otherwise
 */
bool	check_options(char *args[])
{
	int	i;

	i = 0;
	while (args[++i] != NULL)
	{
		if (args[i][0] == '-' && args[i][1] != '\0')
		{
			args[i][2] = '\0';
			export_print_error("invalid option", args[i], true);
			return (true);
		}
	}
	return (false);
}

bool	is_valid_key(const char *str)
{
	int	i;

	if (str == NULL || str[0] == '\0' || !(ft_isalpha((unsigned char) str[0])))
		return (false);
	i = 0;
	while (str[++i] && str[i] != '=')
	{
		if (ft_isalnum((unsigned char) str[i]) == 0)
		{
			if (!(str[i] == '+' && str[i + 1] == '='))
				return (false);
		}
	}
	return (true);
}

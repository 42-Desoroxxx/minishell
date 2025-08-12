/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:30:00 by llage             #+#    #+#             */
/*   Updated: 2025/08/12 18:30:00 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <minishell.h>

static void	print_array(const char *name, char **array)
{
	int	i;

	ft_printf(name);
	i = 0;
	while (array[i] != NULL)
	{
		ft_printf(" %s", array[i]);
		i++;
	}
}

void	print_tokens(const t_token token)
{
	const char	*token_type[] = {"EMPTY", "WORD", "REDIR", "PIPE"};
	t_token		current;

	current = token;
	ft_printf(ANSI_GREEN SHELL_NAME " [Debug]: Tokens:\n" ANSI_RESET);
	while (current.next != NULL)
	{
		ft_printf(ANSI_GREEN "type: %s value: %s ", token_type[current.type], current.value);
		if (current.prev != NULL)
			ft_printf(" hasPrevious");
		if (current.next != NULL)
			ft_printf(" hasNext");
		ft_printf("\n" ANSI_RESET);
		current = *(current.next);
	}
}

void	print_cmd_table(const t_cmd *cmd_table)
{
	t_cmd	current;
	int		i;

	ft_printf(ANSI_GREEN SHELL_NAME " [Debug]: Command Table:\n" ANSI_RESET);
	i = 0;
	current = cmd_table[i];
	while (current.args != NULL)
	{
		ft_printf(ANSI_GREEN "[%d] ", i);
		if (current.args != NULL)
			print_array(" args:", current.args);
		if (current.in_redirs != NULL)
			print_array(" in_redirs:", current.in_redirs);
		if (current.out_redirs != NULL)
			print_array(" out_redirs:", current.out_redirs);
		ft_printf("\n" ANSI_RESET);
		current = cmd_table[++i];
	}
}

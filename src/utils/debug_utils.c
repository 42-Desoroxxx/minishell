/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:30:00 by llage             #+#    #+#             */
/*   Updated: 2025/08/26 06:09:26 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	print_char_array(const char *name, char **array)
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
		ft_printf(ANSI_GREEN "    type: %s value: %s ",
			token_type[current.type], current.value);
		if (current.prev != NULL)
			ft_printf("hasPrevious");
		if (current.next != NULL && current.next->type != EMPTY)
			ft_printf(" hasNext");
		ft_printf("\n" ANSI_RESET);
		current = *(current.next);
	}
}

void	print_cmd_table(const t_cmd_table *cmd_table)
{
	t_cmd	current;
	size_t	i;

	ft_printf(ANSI_GREEN SHELL_NAME " [Debug]: Command Table:\n" ANSI_RESET);
	i = -1;
	while (++i < cmd_table->size)
	{
		current = cmd_table->cmds[i];
		ft_printf(ANSI_GREEN "    [%d]", i);
		print_char_array(" args:", current.args);
		ft_printf(" in_redirs: %d out_redirs: %d \n" ANSI_RESET,
			current.in_redir, current.out_redir);
	}
}

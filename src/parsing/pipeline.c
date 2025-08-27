/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 00:32:09 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/27 00:32:11 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	pipe_my_line(t_cmd *cmd_table, int cmd_count)
{
	int	i;
	int	pipefd[2];

	i = 0;
	while (i <= cmd_count - 1)
	{
		if (pipe(pipefd) == -1)
			return (false);
		if (cmd_table[i].out_redir == 0)
			cmd_table[i].out_redir = pipefd[0];
		else
			close(pipefd[0]);
		if (i <= cmd_count - 2 && cmd_table[i + 1].in_redir == 0)
			cmd_table[i + 1].in_redir = pipefd[1];
		else
			close(pipefd[1]);
		i++;
	}
	return (true);
}

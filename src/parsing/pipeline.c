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

bool	pipe_my_line(t_cmd_table *cmd_table)
{
	size_t	i;
	int		pipes[2];

	i = -1;
	while (++i < cmd_table->size - 1)
	{
		if (pipe(pipes) == -1)
			return (false);
		if (cmd_table->cmds[i].out_redir == 0)
			cmd_table->cmds[i].out_redir = pipes[1];
		else
			close(pipes[1]);
		if (cmd_table->cmds[i + 1].in_redir == 0)
			cmd_table->cmds[i + 1].in_redir = pipes[0];
		else
			close(pipes[0]);
	}
	return (true);
}

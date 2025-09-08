/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 05:35:58 by llage             #+#    #+#             */
/*   Updated: 2025/09/08 05:35:58 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	close_fds(t_cmd *cmd)
{
	if (cmd->in_redir > 0)
	{
		close(cmd->in_redir);
		cmd->in_redir = 0;
	}
	if (cmd->out_redir > 0)
	{
		close(cmd->out_redir);
		cmd->out_redir = 0;
	}
}

void	close_all_fds(t_cmd_table *cmd_table)
{
	size_t	i;

	i = -1;
	while (++i < cmd_table->size)
		close_fds(&cmd_table->cmds[i]);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 00:04:34 by llage             #+#    #+#             */
/*   Updated: 2025/09/05 14:43:38 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <minishell.h>

static void	wait_for_childs(pid_t *pids, size_t pid_count, t_shell *shell)
{
	int		status;
	size_t	i;

	i = -1;
	while (++i < pid_count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
		{
			shell->exit_status = errno;
			return ;
		}
		if (i != pid_count - 1)
			continue ;
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
	}
}

/**
 * @return true if nothing needs to be further executed, false otherwise
 */
static bool	exec_single_cmd(t_cmd *cmd, t_shell *shell, t_cmd_table *cmd_table)
{
	if (cmd->args == NULL || cmd->args[0] == NULL)
	{
		shell->exit_status = 0;
		if (cmd->in_redir == -1 || cmd->out_redir == -1)
			shell->exit_status = 1;
		return (true);
	}
	if (is_builtin(cmd))
	{
		exec_lonely_builtin(cmd, shell);
		if (ft_str_equal(cmd->args[0], "exit"))
		{
			if (cmd->args[1] != NULL && cmd->args[2] != NULL)
				return (true);
			free_cmd_table((t_cmd_table **) &cmd_table);
			rl_clear_history();
			map_free(&shell->env);
			exit(shell->exit_status);
		}
		return (true);
	}
	return (false);
}

static pid_t	exec_cmd(t_cmd *cmd, t_shell *shell, pid_t *pids,
	t_cmd_table *cmd_table)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		shell->exit_status = errno;
		perror(SHELL_NAME);
		return (-1);
	}
	if (pid == 0)
	{
		setup_child(cmd, shell, pids, cmd_table);
		if (is_builtin(cmd))
			child_builtin(cmd, shell, pids, cmd_table);
		else
			child_external(cmd, shell, pids, cmd_table);
	}
	close_fds(cmd);
	return (pid);
}

void	exec_table(t_cmd_table *cmd_table, t_shell *shell)
{
	pid_t	*pids;
	size_t	i;

	if (cmd_table->size == 1)
		if (exec_single_cmd(&cmd_table->cmds[0], shell, cmd_table))
			return ;
	pids = ft_calloc(cmd_table->size + 1, sizeof(pid_t));
	if (pids == NULL)
	{
		perror(SHELL_NAME);
		return ;
	}
	i = -1;
	while (++i < cmd_table->size)
	{
		pids[i] = exec_cmd(&cmd_table->cmds[i], shell, pids, cmd_table);
		if (pids[i] == -1)
		{
			close_all_fds(cmd_table);
			break ;
		}
	}
	wait_for_childs(pids, i, shell);
	free(pids);
}

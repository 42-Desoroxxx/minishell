/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 05:29:52 by llage             #+#    #+#             */
/*   Updated: 2025/09/08 05:29:52 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	is_builtin(const t_cmd *cmd)
{
	const char	*builtins[] = {"echo", "cd", "pwd", "export", "unset",
		"env", "exit", NULL};
	int			i;

	i = -1;
	while (builtins[++i] != NULL)
		if (ft_str_equal(cmd->args[0], builtins[i]))
			return (true);
	return (false);
}

int	exec_builtin(t_cmd *cmd, t_shell *shell)
{
	if (ft_str_equal(cmd->args[0], "echo"))
		return (echo(cmd->args));
	if (ft_str_equal(cmd->args[0], "cd"))
		return (cd(cmd->args, &shell->env));
	if (ft_str_equal(cmd->args[0], "pwd"))
		return (pwd(cmd->args));
	if (ft_str_equal(cmd->args[0], "export"))
		return (export(cmd->args, &shell->env));
	if (ft_str_equal(cmd->args[0], "unset"))
		return (unset(cmd->args, &shell->env));
	if (ft_str_equal(cmd->args[0], "env"))
		return (env(cmd->args, &shell->env));
	if (ft_str_equal(cmd->args[0], "exit"))
		return (ms_exit(cmd->args, shell));
	ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
		" [Error]: command not found (%s)\n" ANSI_RESET, cmd->args[0]);
	return (127);
}

static void	restore_redirs(t_cmd *cmd, int fd_in_backup, int fd_out_backup)
{
	if (fd_in_backup > 0)
	{
		dup2(fd_in_backup, STDIN_FILENO);
		close(fd_in_backup);
	}
	if (fd_out_backup > 0)
	{
		dup2(fd_out_backup, STDOUT_FILENO);
		close(fd_out_backup);
	}
	close_fds(cmd);
}

void	exec_lonely_builtin(t_cmd *cmd, t_shell *shell)
{
	int		fd_in_backup;
	int		fd_out_backup;

	fd_in_backup = -1;
	fd_out_backup = -1;
	if (cmd->in_redir == -1 || cmd->out_redir == -1)
	{
		perror(SHELL_NAME);
		shell->exit_status = 1;
		return ;
	}
	if (cmd->in_redir > 0)
	{
		fd_in_backup = dup(STDIN_FILENO);
		dup2(cmd->in_redir, STDIN_FILENO);
	}
	if (cmd->out_redir > 0)
	{
		fd_out_backup = dup(STDOUT_FILENO);
		dup2(cmd->out_redir, STDOUT_FILENO);
	}
	shell->exit_status = exec_builtin(cmd, shell);
	restore_redirs(cmd, fd_in_backup, fd_out_backup);
}

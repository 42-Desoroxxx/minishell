/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 00:04:34 by llage             #+#    #+#             */
/*   Updated: 2025/08/31 21:37:39 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <minishell.h>

static bool	is_builtin(const t_cmd cmd)
{
	const char	*builtins[] = {"echo", "cd", "pwd", "export", "unset",
		"env", "exit", NULL};
	int			i;

	i = -1;
	while (builtins[++i] != NULL)
		if (ft_str_equal(cmd.args[0], builtins[i]))
			return (true);
	return (false);
}

static int	exec_builtin(t_cmd cmd, t_shell shell)
{
	if (ft_str_equal(cmd.args[0], "echo"))
		return (echo(cmd.args));
	if (ft_str_equal(cmd.args[0], "pwd"))
		return (pwd(cmd.args));
	if (ft_str_equal(cmd.args[0], "env"))
	{
		map_print(&shell.env);
		return (0);
	}
	ft_fprintf(STDERR_FILENO, SHELL_NAME ANSI_RED
		" [Error]: Builtin command not implemented\n" ANSI_RESET);
	return (1);
}

void	exec_table(t_cmd *cmd_table, t_shell *shell)
{
	t_cmd	current;
	int		i;
	pid_t	pid;
	char	**envp;
	int		status;
	int		last_status;
	char	*path;

	i = 0;
	current = cmd_table[i];
	if (cmd_table[1].args == NULL && is_builtin(current))
	{
		int	fd_in_save = dup(STDIN_FILENO);
		int	fd_out_save = dup(STDOUT_FILENO);

		if (current.in_redir > 0)
			dup2(current.in_redir, STDIN_FILENO);
		if (current.out_redir > 0)
			dup2(current.out_redir, STDOUT_FILENO);

		status = exec_builtin(current, *shell);

		if (current.in_redir > 0)
		{
			dup2(fd_in_save, STDIN_FILENO);
			close(current.in_redir);
		}
		if (current.out_redir > 0)
		{
			dup2(fd_out_save, STDOUT_FILENO);
			close(current.out_redir);
		}

		shell->exit_status = status;
		return ;
	}
	while (current.args != NULL)
	{
		if (is_builtin(current))
		{
			pid = fork();
			if (pid < 0)
			{
				shell->exit_status = errno;
				return ;
			}
			if (pid == 0)
			{
				if (current.in_redir > 0)
				{
					dup2(current.in_redir, STDIN_FILENO);
					close(current.in_redir);
				}
				if (current.out_redir > 0)
				{
					dup2(current.out_redir, STDOUT_FILENO);
					close(current.out_redir);
				}
				exit(exec_builtin(current, *shell));
			}
			if (waitpid(pid, &status, 0) == -1)
			{
				shell->exit_status = errno;
				return ;
			}
		}
		else
		{
			if (ft_strchr(current.args[0], '/') == NULL)
				path = find_in_path(shell->env, current.args[0]);
			else
				path = ft_strdup(current.args[0]);
			if (path == NULL)
			{
				ft_fprintf(STDERR_FILENO, SHELL_NAME
					": command not found (%s)\n", current.args[0]);
				shell->exit_status = 127;
				return ;
			}
			envp = create_envp(shell->env);
			pid = fork();
			if (pid < 0)
			{
				shell->exit_status = errno;
				return ;
			}
			if (pid == 0)
			{
				if (current.in_redir > 0)
				{
					dup2(current.in_redir, STDIN_FILENO);
					close(current.in_redir);
				}
				if (current.out_redir > 0)
				{
					dup2(current.out_redir, STDOUT_FILENO);
					close(current.out_redir);
				}
				execve(path, current.args, envp);
				if (errno == ENOENT)
					exit(127);
				if (errno == EACCES)
					exit(126);
				exit(1);
			}
			if (waitpid(pid, &status, 0) == -1)
			{
				free_envp(&envp);
				free(path);
				shell->exit_status = errno;
				return ;
			}
			free_envp(&envp);
			free(path);
		}
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
		current = cmd_table[++i];
	}
	shell->exit_status = last_status;
}

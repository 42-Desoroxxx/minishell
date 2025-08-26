/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 00:04:34 by llage             #+#    #+#             */
/*   Updated: 2025/08/26 07:00:18 by llage            ###   ########.fr       */
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

static int	exec_builtin(t_cmd cmd, t_map env)
{
	(void)cmd;
	(void)env;
	ft_fprintf(STDERR_FILENO, SHELL_NAME
		" [Error]: Builtin command not implemented\n");
	return (1);
}

int	exec_table(t_cmd *cmd_table, t_map env)
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
	while (current.args != NULL)
	{
		if (is_builtin(current))
		{
			pid = fork();
			if (pid < 0)
				return (errno);
			if (pid == 0)
			{
				exec_builtin(current, env);
				exit(1);
			}
			if (waitpid(pid, &status, 0) == -1)
				return (errno);
		}
		else
		{
			if (ft_strchr(current.args[0], '/') == NULL)
				path = find_in_path(env, current.args[0]);
			else
				path = ft_strdup(current.args[0]);
			if (path == NULL)
			{
				ft_fprintf(STDERR_FILENO, SHELL_NAME
					": command not found (%s)\n", current.args[0]);
				return (127);
			}
			envp = create_envp(env);
			pid = fork();
			if (pid < 0)
				return (errno);
			if (pid == 0)
			{
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
				return (errno);
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
	return (last_status);
}

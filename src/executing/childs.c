/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 05:27:09 by llage             #+#    #+#             */
/*   Updated: 2025/09/08 05:27:09 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_child(pid_t *pids, t_cmd_table *cmd_table, t_map *env)
{
	free(pids);
	free_cmd_table(&cmd_table);
	map_free(env);
}

void	setup_child(t_cmd *cmd, t_shell *shell, pid_t *pids,
	t_cmd_table *cmd_table)
{
	size_t	i;

	if (cmd->in_redir > 0)
		dup2(cmd->in_redir, STDIN_FILENO);
	if (cmd->out_redir > 0)
		dup2(cmd->out_redir, STDOUT_FILENO);
	i = -1;
	while (++i < cmd_table->size)
		close_fds(&cmd_table->cmds[i]);
	if (cmd->in_redir == -1 || cmd->out_redir == -1)
	{
		perror(SHELL_NAME);
		free_child(pids, cmd_table, &shell->env);
		exit(1);
	}
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}

void	child_builtin(t_cmd *cmd, t_shell *shell, pid_t *pids,
	t_cmd_table *cmd_table)
{
	int status;

	status = exec_builtin(cmd, shell);
	free_child(pids, cmd_table, &shell->env);
	exit(status);
}

static char	*get_path(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	path = NULL;
	if (map_get(&shell->env, "PATH") != NULL
		&& cmd->args[0][0] != '\0' && ft_strchr(cmd->args[0], '/') == NULL)
		path = find_in_path(shell->env, cmd->args[0]);
	else if (cmd->args[0][0] != '\0')
		path = ft_strdup(cmd->args[0]);
	if (path == NULL || path[0] == '\0')
	{
		ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME " [Error]: "
			"command not found (%s)\n" ANSI_RESET, cmd->args[0]);
		shell->exit_status = 127;
		return (NULL);
	}
	return (path);
}

void	child_external(t_cmd *cmd, t_shell *shell, pid_t *pids,
	t_cmd_table *cmd_table)
{
	char	**envp;
	char	*path;

	if (cmd->args == NULL || cmd->args[0] == NULL)
	{
		free_child(pids, cmd_table, &shell->env);
		exit(0);
	}
	path = get_path(cmd, shell);
	if (path == NULL)
	{
		free_child(pids, cmd_table, &shell->env);
		exit(127);
	}
	envp = create_envp(shell->env);
	if (envp == NULL)
	{
		free_child(pids, cmd_table, &shell->env);
		free(path);
		exit(1);
	}
	execve(path, cmd->args, envp);
	if (errno == ENOEXEC)
	{
		size_t argc = 0;
		while (cmd->args[argc] != NULL)
			argc++;
		char **sh_argv = malloc((argc + 2) * sizeof(char *));
		if (sh_argv == NULL)
		{
			free(path);
			exit(1);
		}
		sh_argv[0] = "sh";
		sh_argv[1] = path;
		for (size_t k = 1; k < argc; k++)
			sh_argv[k + 1] = cmd->args[k];
		sh_argv[argc + 1] = NULL;
		execve("/bin/sh", sh_argv, envp);
		free(sh_argv);
	}
	free_child(pids, cmd_table, &shell->env);
	free_envp(&envp);
	free(path);
	if (errno == ENOENT)
		exit(127);
	exit(126);
}

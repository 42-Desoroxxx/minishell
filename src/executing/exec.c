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

static bool	is_builtin(const t_cmd *cmd)
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

static int	exec_builtin(t_cmd *cmd, t_shell *shell)
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
	{
		ft_fprintf(STDERR_FILENO, "exit");
		exit((uint8_t) ft_atoi(cmd->args[1]));
	}
	return (1);
}

static void	restore_redirs(t_cmd *cmd, int fd_in_backup, int fd_out_backup)
{
	if (fd_in_backup > 0)
	{
		dup2(fd_in_backup, STDIN_FILENO);
		close(fd_in_backup);
		close(cmd->in_redir);
		cmd->in_redir = 0;
	}
	if (fd_out_backup > 0)
	{
		dup2(fd_out_backup, STDOUT_FILENO);
		close(fd_out_backup);
		close(cmd->out_redir);
		cmd->out_redir = 0;
	}
}

static void	exec_lonely_builtin(t_cmd *cmd, t_shell *shell)
{
	int		fd_in_backup;
	int		fd_out_backup;

	fd_in_backup = -1;
	fd_out_backup = -1;
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

void	exec_table(t_cmd_table *cmd_table, t_shell *shell)
{
    t_cmd	*current;
    size_t	i;
    pid_t	*pids;
    int		npids = 0;
    int		status, last_status = 0;

    current = &cmd_table->cmds[0];
    if (cmd_table->size == 1 && is_builtin(current))
    {
    	exec_lonely_builtin(current, shell);
        return ;
    }
	pids = ft_calloc(cmd_table->size + 1, sizeof(pid_t));
	if (pids == NULL)
	{
		perror(SHELL_NAME);
		return ;
	}
	i = -1;
    while (++i < cmd_table->size)
    {
        pid_t pid;

        current = &cmd_table->cmds[i];
        if (is_builtin(current))
        {
            pid = fork();
            if (pid < 0)
            {
                shell->exit_status = errno;
            	free(pids);
                return ;
            }
            if (pid == 0)
            {
            	signal(SIGQUIT, SIG_DFL);
            	signal(SIGINT, SIG_DFL);
                if (current->in_redir > 0)
                    dup2(current->in_redir, STDIN_FILENO);
                if (current->out_redir > 0)
                    dup2(current->out_redir, STDOUT_FILENO);
            	for (size_t j = 0; j < cmd_table->size; j++)
            	{
            		if (cmd_table->cmds[j].in_redir > 0)
            			close(cmd_table->cmds[j].in_redir);
            		if (cmd_table->cmds[j].out_redir > 0)
            			close(cmd_table->cmds[j].out_redir);
            	}
                exit(exec_builtin(current, shell));
            }
            // parent: close our copies so readers see EOF
            if (current->in_redir > 0)
			{
				close(current->in_redir);
            	current->in_redir = 0;
			}
            if (current->out_redir > 0)
			{
				close(current->out_redir);
            	current->out_redir = 0;
			}

            pids[npids++] = pid;
        }
        else
        {
            char **envp;
            char *path;

        	if (current->args == NULL || current->args[0] == NULL)
        		continue ;
        	path = NULL;
            if (current->args[0][0] != '\0' && ft_strchr(current->args[0], '/') == NULL)
                path = find_in_path(shell->env, current->args[0]);
            else if (current->args[0][0] != '\0')
                path = ft_strdup(current->args[0]);
            if (path == NULL || path[0] == '\0')
            {
                ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME "[Error]: "
					"command not found (%s)\n" ANSI_RESET, current->args[0]);
                shell->exit_status = 127;
            	free(pids);
                return ;
            }

            envp = create_envp(shell->env);
            pid = fork();
            if (pid < 0)
            {
                free(path);
                free_envp(&envp);
                shell->exit_status = errno;
            	perror(SHELL_NAME);
            	free(pids);
                return ;
            }
            if (pid == 0)
            {
				signal(SIGQUIT, SIG_DFL);
				signal(SIGINT, SIG_DFL);
				if (current->in_redir > 0)
                    dup2(current->in_redir, STDIN_FILENO);
                if (current->out_redir > 0)
                    dup2(current->out_redir, STDOUT_FILENO);
            	for (size_t j = 0; j < cmd_table->size; j++)
            	{
            		if (cmd_table->cmds[j].in_redir > 0)
            			close(cmd_table->cmds[j].in_redir);
            		if (cmd_table->cmds[j].out_redir > 0)
            			close(cmd_table->cmds[j].out_redir);
            	}
                execve(path, current->args, envp);
                if (errno == ENOENT)
                    exit(127);
                if (errno == EACCES)
                    exit(126);
                exit(1);
            }
            // parent: close our copies immediately
            if (current->in_redir > 0)
			{
				close(current->in_redir);
            	current->in_redir = 0;
			}
            if (current->out_redir > 0)
            {
	            close(current->out_redir);
            	current->out_redir = 0;
            }

            pids[npids++] = pid;

            free_envp(&envp);
            free(path);
        }
    }


    for (int j = 0; j < npids; j++)
    {
        if (waitpid(pids[j], &status, 0) == -1)
        {
            shell->exit_status = errno;
        	free(pids);
            return ;
        }
        if (j == npids - 1)
        {
            if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_status = 128 + WTERMSIG(status);
        }
    }

    shell->exit_status = last_status;
	free(pids);
}


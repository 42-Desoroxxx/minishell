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

static int	exec_builtin(t_cmd cmd, t_shell *shell)
{
	if (ft_str_equal(cmd.args[0], "echo"))
		return (echo(cmd.args));
	if (ft_str_equal(cmd.args[0], "pwd"))
		return (pwd(cmd.args));
	if (ft_str_equal(cmd.args[0], "unset"))
		return (unset(cmd.args, &shell->env));
	if (ft_str_equal(cmd.args[0], "env"))
	{
		map_print(&shell->env);
		return (0);
	}
	ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
		" [Error]: Builtin command not implemented\n" ANSI_RESET);
	return (1);
}

void exec_table(t_cmd_table *cmd_table, t_shell *shell)
{
    t_cmd  current;
    size_t    i;
    pid_t  *pids;
    int    npids = 0;
    int    status, last_status = 0;

    pids = ft_calloc(cmd_table->size + 1, sizeof(pid_t));
    if (pids == NULL)
    {
        shell->exit_status = errno;
    	free(pids);
        return ;
    }
    current = cmd_table->cmds[0];
    if (cmd_table->size == 1 && is_builtin(current))
    {
        int fd_in_save, fd_out_save;

        if (current.in_redir > 0)
        {
            fd_in_save = dup(STDIN_FILENO);
            dup2(current.in_redir, STDIN_FILENO);
        }
        if (current.out_redir > 0)
        {
            fd_out_save = dup(STDOUT_FILENO);
            dup2(current.out_redir, STDOUT_FILENO);
        }

        status = exec_builtin(current, shell);

        if (current.in_redir > 0)
        {
            dup2(fd_in_save, STDIN_FILENO);
            close(fd_in_save);
            close(current.in_redir);
        	cmd_table->cmds[0].in_redir = 0;
        }
        if (current.out_redir > 0)
        {
            dup2(fd_out_save, STDOUT_FILENO);
            close(fd_out_save);
            close(current.out_redir);
        	cmd_table->cmds[0].out_redir = 0;
        }

        shell->exit_status = status;
    	free(pids);
        return ;
    }
	i = -1;
    while (++i < cmd_table->size)
    {
        pid_t pid;

        current = cmd_table->cmds[i];
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
                exit(exec_builtin(current, shell)); // ? possibly problematic since exit != _exit
            	signal(SIGQUIT, SIG_DFL);
            	signal(SIGINT, SIG_DFL);
            }
            // parent: close our copies so readers see EOF
            if (current.in_redir > 0)
			{
				close(current.in_redir);
            	cmd_table->cmds[i].in_redir = 0;
			}
            if (current.out_redir > 0)
			{
				close(current.out_redir);
            	cmd_table->cmds[i].out_redir = 0;
			}

            pids[npids++] = pid;
        }
        else
        {
            char **envp;
            char *path;

            if (ft_strchr(current.args[0], '/') == NULL)
                path = find_in_path(shell->env, current.args[0]);
            else
                path = ft_strdup(current.args[0]);
            if (path == NULL)
            {
                ft_fprintf(STDERR_FILENO, SHELL_NAME ": command not found (%s)\n", current.args[0]);
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
            	free(pids);
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
				signal(SIGQUIT, SIG_DFL);
				signal(SIGINT, SIG_DFL);
                if (errno == ENOENT)
                    exit(127);
                if (errno == EACCES)
                    exit(126);
                exit(1);
            }
            // parent: close our copies immediately
            if (current.in_redir > 0)
			{
				close(current.in_redir);
            	cmd_table->cmds[i].in_redir = 0;
			}
            if (current.out_redir > 0)
            {
	            close(current.out_redir);
            	cmd_table->cmds[i].out_redir = 0;
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


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 06:15:50 by llage             #+#    #+#             */
/*   Updated: 2025/09/08 06:15:50 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_envp(char ***envp_ptr)
{
	char	**envp;
	int		i;

	if (envp_ptr == NULL || *envp_ptr == NULL)
		return ;
	i = -1;
	envp = *envp_ptr;
	while (envp[++i])
	{
		free(envp[i]);
		envp[i] = NULL;
	}
	free(envp);
	*envp_ptr = NULL;
}

void	free_external_child(pid_t *pids, t_cmd_table *cmd_table,
	t_map *env, char *path)
{
	free_child(pids, cmd_table, env);
	free(path);
}

char	**create_envp(t_map *env, pid_t *pids, t_cmd_table *cmd_table,
	char *path)
{
	char	**envp;
	size_t	i;

	envp = ft_calloc(env->size + 1, sizeof(char *));
	if (envp == NULL)
	{
		perror(SHELL_NAME);
		free_external_child(pids, cmd_table, env, path);
		exit(1);
	}
	i = -1;
	while (++i < env->size)
	{
		envp[i] = ft_str_add(envp[i], env->entries[i].key);
		envp[i] = ft_str_add(envp[i], "=");
		envp[i] = ft_str_add(envp[i], env->entries[i].value);
		if (envp[i] == NULL)
		{
			perror(SHELL_NAME);
			free_external_child(pids, cmd_table, env, path);
			free_envp(&envp);
			exit(1);
		}
	}
	return (envp);
}

bool	exec_sh(t_cmd *cmd, char *path, char **envp)
{
	char	**sh_argv;
	size_t	argc;
	size_t	i;

	argc = 0;
	while (cmd->args[argc] != NULL)
		argc++;
	sh_argv = ft_calloc(argc + 2, sizeof(char *));
	if (sh_argv == NULL)
		return (false);
	sh_argv[0] = "sh";
	sh_argv[1] = path;
	i = 1;
	while (i < argc)
	{
		sh_argv[i + 1] = cmd->args[i];
		i++;
	}
	execve("/bin/sh", sh_argv, envp);
	free(sh_argv);
	return (false);
}

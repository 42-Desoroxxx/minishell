/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:27:04 by llage             #+#    #+#             */
/*   Updated: 2025/08/26 05:52:05 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	lvl_to_level(int lvl, char *new_level)
{
	ft_bzero(new_level, 5);
	if (lvl < 10)
		new_level[0] = lvl + '0';
	else if (lvl < 100)
	{
		new_level[0] = (lvl / 10) + '0';
		new_level[1] = (lvl % 10) + '0';
	}
	else if (lvl < 1000)
	{
		new_level[0] = (lvl / 100) + '0';
		new_level[1] = ((lvl % 100) / 10) + '0';
		new_level[2] = ((lvl % 100) % 10) + '0';
	}
}

static void	adjust_shell_level(t_map *env)
{
	const char	*old_level = map_get(env, "SHLVL");
	char		new_level[5];
	int			lvl;

	if (old_level != NULL)
		lvl = ft_atoi(old_level) + 1;
	else
		lvl = 0;
	if (lvl >= 1000)
	{
		ft_fprintf(STDERR_FILENO, SHELL_NAME
			" [Warning]: Shell level (%d) too high, resetting to 1\n", lvl);
		lvl = 1;
	}
	else if (lvl < 0)
	{
		ft_fprintf(STDERR_FILENO, SHELL_NAME
			" [Warning]: Shell level (%d) too low, resetting to 0\n", lvl);
		lvl = 0;
	}
	lvl_to_level(lvl, new_level);
	map_set(env, "SHLVL", new_level);
}

t_map	create_env(char *envp[])
{
	char	*value;
	t_map	env;
	int		i;

	env = map_bzero();
	if (envp == NULL || envp[0] == NULL)
		return (env);
	i = 0;
	while (envp[i])
	{
		value = ft_strchr(envp[i], '=');
		*value = '\0';
		if (!map_set(&env, envp[i], ++value))
		{
			map_free(&env);
			env.size = -1;
			return (env);
		}
		i++;
	}
	adjust_shell_level(&env);
	return (env);
}

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

char	**create_envp(t_map env)
{
	char	**envp;
	size_t	i;

	envp = ft_calloc(env.size + 1, sizeof(char *));
	if (envp == NULL)
	{
		perror(SHELL_NAME);
		return (NULL);
	}
	i = -1;
	while (++i < env.size)
	{
		envp[i] = ft_str_add(envp[i], env.entries[i].key);
		envp[i] = ft_str_add(envp[i], "=");
		envp[i] = ft_str_add(envp[i], env.entries[i].value);
		if (envp[i] == NULL)
		{
			perror(SHELL_NAME);
			free_envp(&envp);
			return (NULL);
		}
	}
	return (envp);
}

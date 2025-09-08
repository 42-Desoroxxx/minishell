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

static void	adjust_shell_level(t_map *env)
{
	const char	*old_level = map_get(env, "SHLVL");
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
	map_set(env, "SHLVL", ft_byte_to_str(lvl));
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 23:21:32 by llage             #+#    #+#             */
/*   Updated: 2025/09/04 23:21:32 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*get_home(t_map *env)
{
	char	*home;
	char	*tmp;

	home = map_get(env, "HOME");
	tmp = NULL;
	if (home != NULL)
	{
		tmp = ft_strtrim(home, " ");
		if (tmp == NULL)
		{
			perror(SHELL_NAME);
			return (NULL);
		}
	}
	if (home == NULL || home[0] == '\0' || ft_str_equal(tmp, "/"))
	{
		cd_print_error("HOME not set");
		if (tmp != NULL)
			free(tmp);
		return (NULL);
	}
	free(tmp);
	return (home);
}

static char	*expand_tilde(char *target, t_map *env)
{
	const char	*home = get_home(env);
	char		*tmp;

	if (home == NULL)
		return (NULL);
	tmp = ft_strjoin(home, target + 1);
	return (tmp);
}

static void	set_pwds(char *old_cwd, t_map *env)
{
	char	*cwd;

	map_set(env, "OLDPWD", old_cwd);
	cwd = getcwd(NULL, 0);
	map_set(env, "PWD", cwd);
	free(cwd);
}

static char	*get_target(char *arg, t_map *env)
{
	if (arg == NULL)
	{
		if (get_home(env) == NULL)
			return (NULL);
		return (ft_strdup(get_home(env)));
	}
	if (arg[0] == '~')
		return (expand_tilde(arg, env));
	return (ft_strdup(arg));
}

int	cd(char *args[], t_map *env)
{
	char	*target;
	char	*cwd;

	if (args[1] != NULL && args[2] != NULL)
	{
		cd_print_error("too many arguments");
		return (1);
	}
	target = get_target(args[1], env);
	if (target == NULL)
		return (1);
	cwd = getcwd(NULL, 0);
	if (chdir(target) == -1)
	{
		perror(SHELL_NAME " [Error]: cd");
		free(target);
		free(cwd);
		return (1);
	}
	set_pwds(cwd, env);
	free(target);
	free(cwd);
	return (0);
}

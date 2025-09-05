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

static void	print_error(const char *name)
{
	ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
		" [Error]: cd: %s\n" ANSI_RESET, name);
}

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
		print_error("HOME not set");
		return (NULL);
	}
	free(tmp);
	return (home);
}

static void	set_pwds(char *old_cwd, t_map *env)
{
	char	*cwd;

	map_set(env, "OLDPWD", old_cwd);
	cwd = getcwd(NULL, 0);
	map_set(env, "PWD", cwd);
	free(cwd);
}

int	cd(char *args[], t_map *env)
{
	char	*target;
	char	*cwd;

	if (args[1] != NULL && args[2] != NULL)
	{
		print_error("too many arguments");
		return (1);
	}
	target = args[1];
	if (target == NULL)
		target = get_home(env);
	if (target == NULL)
		return (1);
	cwd = getcwd(NULL, 0);
	if (chdir(target) == -1)
	{
		perror(SHELL_NAME " [Error]: cd:");
		free(cwd);
		return (1);
	}
	set_pwds(cwd, env);
	free(cwd);
	return (0);
}

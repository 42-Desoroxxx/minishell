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

int	cd(char *args[], t_map *env)
{
	char	*cwd;

	if (args[2] != NULL)
	{
		ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
			"[Error]: cd: too many arguments\n" ANSI_RESET);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (chdir(args[1]) == -1)
	{
		perror("cd");
		free(cwd);
		return (1);
	}
	map_set(env, "OLDPWD", cwd);
	free(cwd);
	cwd = getcwd(NULL, 0);
	map_set(env, "PWD", cwd);
	free(cwd);
	return (0);
}

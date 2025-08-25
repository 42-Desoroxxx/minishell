/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 08:39:07 by llage             #+#    #+#             */
/*   Updated: 2025/08/25 08:42:58 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	free_path_list(char **path_list)
{
	int	i;

	i = -1;
	while (path_list[++i] != NULL)
		free(path_list[i]);
	free(path_list);
}

/*
 * Ensures the path ends with a slash.
 *
 * @returns the newly allocated path to use or NULL in case of errors.
 */
static char	*ensure_slash(char *path)
{
	char	*temp;

	if (path[ft_strlen(path) - 1] == '/')
		return (ft_strdup(path));
	temp = ft_strjoin(path, "/");
	if (temp == NULL)
	{
		perror(SHELL_NAME);
		return (NULL);
	}
	return (temp);
}

/*
 * Checks if the executable is in this path.
 *
 * @returns the absolute path to the executable or NULL if none was found.
 */
static char	*check_path(char *path, char *name)
{
	char	*result;

	path = ensure_slash(path);
	if (path == NULL)
		return (NULL);
	result = ft_strjoin(path, name);
	free(path);
	if (result == NULL)
	{
		perror(SHELL_NAME);
		return (NULL);
	}
	if (access(result, X_OK) == 0)
		return (result);
	free(result);
	return (NULL);
}

/*
 * Finds an executable in the PATH environment variable.
 *
 * @returns the absolute path to the executable or NULL if none was found.
 */
char	*find_in_path(t_map env , char *name)
{
	const char	*path = map_get(&env, "PATH");
	char		**path_list;
	char		*result;
	int			i;

	if (path == NULL)
		return (NULL);
	path_list = ft_split(path, ':');
	result = NULL;
	i = -1;
	while (path_list[++i] != NULL)
	{
		result = check_path(path_list[i], name);
		if (result != NULL)
		{
			free_path_list(path_list);
			return (result);
		}
	}
	free_path_list(path_list);
	return (result);
}

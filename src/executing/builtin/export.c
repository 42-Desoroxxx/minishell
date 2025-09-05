/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 23:30:50 by llage             #+#    #+#             */
/*   Updated: 2025/09/05 03:05:23 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	print_error(const char *name, const char *value)
{
	ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
				"[Error]: export: %s: %s\n" ANSI_RESET,
				value, name);
}

/**
 * @return true if an invalid option was found, false otherwise
 */
static bool	check_options(char *args[])
{
	int	i;

	i = 0;
	while (args[++i] != NULL)
	{
		if (args[i][0] == '-' && args[i][1] != '\0')
		{
			args[i][2] = '\0';
			print_error("invalid option", args[i]);
			return (true);
		}
	}
	return (false);
}

static void	print(t_map *env)
{
	(void)env ;
}

static void	process_arg(t_map *env, char *arg, uint8_t *status)
{
	char	*value;

	if (arg[0] == '_')
		return ;
	value = ft_strchr(arg, '=');
	if (value == NULL)
		map_set(env, arg, NULL);
	else if (value[1] == '\0')
		map_set(env, arg, "");
	else
	{
		*value = '\0';
		if (ft_strchr(arg, '?') != NULL || ft_strchr(arg, '-') != NULL)
		{
			*value = '=';
			print_error("not a valid identifier", arg);
			*status = 1;
			return ;
		}
		map_set(env, arg, value + 1);
	}
}

int	export(char *args[], t_map *env)
{
	uint8_t	status;
	int		i;

	if (args[1] == NULL)
	{
		print(env);
		return (0);
	}
	if (check_options(args))
		return (2);
	status = 0;
	i = 0;
	while (args[++i] != NULL)
		process_arg(env, args[i], &status);
	return (status);
}

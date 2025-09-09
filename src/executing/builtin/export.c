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

static void	set_value(t_map *env, char *key, char *value)
{
	bool	append;
	char	*tmp;

	tmp = ft_strchr(key, '+');
	append = tmp != NULL;
	if (tmp != NULL)
		*tmp = '\0';
	*value = '\0';
	if (value[1] == '\0' && !append)
		map_set(env, key, "");
	else
	{
		value++;
		tmp = NULL;
		if (map_get(env, key) != NULL && append)
		{
			tmp = ft_strjoin(map_get(env, key), value);
			if (tmp == NULL)
				return ;
			map_set(env, key, tmp);
			free(tmp);
		}
		else
			map_set(env, key, value);
	}
}

static void	process_arg(t_map *env, char *arg, uint8_t *status)
{
	char	*value;

	if (!is_valid_key(arg))
	{
		export_print_error("not a valid identifier", arg, false);
		*status = 1;
		return ;
	}
	value = ft_strchr(arg, '=');
	if (value == NULL)
	{
		if (map_get(env, arg) != NULL)
			return ;
		map_set(env, arg, NULL);
	}
	else
		set_value(env, arg, value);
}

static void	sort_entries(t_map_entry **entries, size_t n)
{
	t_map_entry	*tmp;
	size_t		i;
	size_t		j;

	i = -1;
	while (++i + 1 < n)
	{
		j = i;
		while (++j < n)
		{
			if (key_cmp(entries[i]->key, entries[j]->key) > 0)
			{
				tmp = entries[i];
				entries[i] = entries[j];
				entries[j] = tmp;
			}
		}
	}
}

static void	print(t_map *env)
{
	t_map_entry	**entries;
	size_t		i;

	entries = ft_calloc(env->size, sizeof(t_map_entry *));
	if (entries == NULL)
		return ;
	i = -1;
	while (++i < env->size)
		entries[i] = &env->entries[i];
	sort_entries(entries, env->size);
	i = -1;
	while (++i < env->size)
	{
		if (entries[i]->value)
			ft_printf("declare -x %s=\"%s\"\n",
				entries[i]->key, entries[i]->value);
		else
			ft_printf("declare -x %s\n",
				entries[i]->key);
	}
	free(entries);
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
	{
		if (ft_strncmp(args[i], "_=", 2) == 0)
			continue ;
		process_arg(env, args[i], &status);
	}
	return (status);
}

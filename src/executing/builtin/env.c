/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:55:10 by llage             #+#    #+#             */
/*   Updated: 2025/09/04 22:55:10 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	env(char *args[], t_map *env)
{
	size_t	i;

	(void)args;
	i = -1;
	while (++i < env->size)
		if (env->entries[i].value != NULL)
			ft_printf("%s=%s\n", env->entries[i].key, env->entries[i].value);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 23:03:54 by llage             #+#    #+#             */
/*   Updated: 2025/09/01 20:04:22 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	pwd(char *args[])
{
	const char	*cwd = getcwd(NULL, 0);

	(void)args;
	ft_printf("%s\n", cwd);
	free((void *)cwd);
	return (0);
}

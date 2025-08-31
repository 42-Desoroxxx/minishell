/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 23:03:54 by llage             #+#    #+#             */
/*   Updated: 2025/08/30 23:03:54 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stdlib.h>

int	pwd(char *args[])
{
	(void)args;
	const char	*cwd = getcwd(NULL, 0);

	ft_printf("%s\n", cwd);
	free((void *)cwd);
	return (0);
}

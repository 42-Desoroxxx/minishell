/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 01:48:37 by llage             #+#    #+#             */
/*   Updated: 2025/09/08 01:48:37 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	cd_print_error(const char *name)
{
	ft_fprintf(STDERR_FILENO, ANSI_RED SHELL_NAME
		" [Error]: cd: %s\n" ANSI_RESET, name);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:27:04 by llage             #+#    #+#             */
/*   Updated: 2025/07/07 17:42:51 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*getprompt(void)
{
	const char	*before = "\001\e[32m\002user@hostname"
		"\001\e[0m\002:\001\e[34m\002";
	const char	*delimiter = "\001\e[0m\002$ ";
	const char	*cwd = getcwd(NULL, 0);
	const int	length = ft_strlen(before) + ft_strlen(cwd)
		+ ft_strlen(delimiter);
	char		*prompt;

	prompt = ft_calloc(length + 1, sizeof(char));
	ft_strlcat(prompt, before, length + 1);
	ft_strlcat(prompt, cwd, length + 1);
	free((void *) cwd);
	ft_strlcat(prompt, delimiter, length + 1);
	return (prompt);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*line;
	char	*prompt;

	(void)argc;
	(void)argv;
	(void)envp;
	while (true)
	{
		prompt = getprompt();
		line = readline(prompt);
		lexer(line);
		free(prompt);
		if (line == NULL)
			break ;
		if (line[0] == '\0')
			continue ;
		if (line[0] != ' ')
			add_history(line);
		ft_printf("%s\n", line);
	}
	exit(EXIT_SUCCESS);
}

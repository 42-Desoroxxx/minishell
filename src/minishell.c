/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:27:04 by llage             #+#    #+#             */
/*   Updated: 2025/06/01 21:36:46 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*getprompt(void)
{
	const char	*before = "\e[32muser@hostname\e[0m:\e[34m";
	const char	*delimiter = "\e[0m$ ";
	int			length;
	char		*prompt;
	char		*cwd;

	cwd = getcwd(NULL, 0);
	length = ft_strlen(before) + ft_strlen(cwd) + ft_strlen(delimiter);
	prompt = ft_calloc(length + 1, sizeof(char));
	ft_strlcat(prompt, before, length + 1);
	ft_strlcat(prompt, cwd, length + 1);
	free(cwd);
	ft_strlcat(prompt, delimiter, length + 1);
	return (prompt);
}

int	main(int argc, char *argv[], char *envp[])
{
	char *line;
	char *prompt;

	(void)argc;
	(void)argv;
	(void)envp;
	while (true)
	{
		prompt = getprompt();
		line = readline(prompt);
		free(prompt);
		if (line == NULL)
			break;
		if (line[0] == '\0')
			continue;
		if (line[0] != ' ') // ? Bash seems to not add things to history if the line starts with a space
			add_history(line);
		ft_printf("%s\n", line);
	}
	exit(EXIT_SUCCESS);
}

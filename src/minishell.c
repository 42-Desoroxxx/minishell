/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:27:04 by llage             #+#    #+#             */
/*   Updated: 2025/08/11 19:08:58 by rvitiell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*getprompt(void)
{
	const char	*at = ANSI_GREEN "user@hostname" ANSI_RESET ":" ANSI_BLUE;
	const char	*delimiter = ANSI_RESET "$ ";
	const char	*cwd = getcwd(NULL, 0);
	const int	length = ft_strlen(at) + ft_strlen(cwd) + ft_strlen(delimiter);
	char		*prompt;

	prompt = ft_calloc(length + 1, sizeof(char));
	ft_strlcat(prompt, at, length + 1);
	ft_strlcat(prompt, cwd, length + 1);
	free((void *) cwd);
	ft_strlcat(prompt, delimiter, length + 1);
	return (prompt);
}

int	main(int argc, char *argv[], char *envp[])
{
	const t_cmd	*cmd_table;
	t_token		*tokens;
	char		*prompt;
	char		*line;

	(void)argc;
	(void)argv;
	(void)envp;
	while (true)
	{
		prompt = getprompt();
		line = readline(prompt);
		free(prompt);
		if (line == NULL)
			break ;
		if (line[0] == '\0')
			continue ;
		tokens = lexer(line);
		if (tokens)
		{
			if (DEBUG)
				print_tokens(*tokens);
			cmd_table = parser(&tokens);
			free_tokens(&tokens);
			if (cmd_table != NULL)
			{
				if (DEBUG)
					print_cmd_table(cmd_table);
			}
		}
		if (line[0] != ' ')
			add_history(line);
		ft_printf(ANSI_GREEN SHELL_NAME " [Debug] line: %s\n", ANSI_RESET, line);
	}
	exit(EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:27:04 by llage             #+#    #+#             */
/*   Updated: 2025/08/25 10:00:37 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*getprompt(void)
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
	const t_map	env = create_env(envp);

	(void)argc;
	(void)argv;
	if (env.size == (size_t) -1)
	{
		perror(SHELL_NAME);
		exit(EXIT_FAILURE);
	}
	if (DEBUG) // TODO: Temporary until we can run export
	{
		ft_printf(ANSI_GREEN);
		map_print(&env);
		ft_printf(ANSI_RESET);
	}
	while (true)
	{
		prompt = getprompt();
		line = readline(prompt);
		free(prompt);
		if (line == NULL)
			break ;
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		if (line[0] != ' ')
			add_history(line);
		ft_printf(ANSI_GREEN SHELL_NAME " [Debug] Line: %s\n" ANSI_RESET,
			line);
		char *absolute_path = find_in_path(env, line);
		ft_printf(ANSI_GREEN SHELL_NAME " [Debug] Absolute path: %s\n",
			absolute_path);
		free(absolute_path);
		tokens = lexer(line, env);
		free(line);
		if (tokens != NULL)
		{
			if (DEBUG)
				print_tokens(*tokens);
			cmd_table = parser(&tokens, env);
			free_tokens(&tokens);
			if (cmd_table != NULL)
			{
				if (DEBUG)
					print_cmd_table(cmd_table);
			}
		}
	}
	map_free((t_map *) &env);
	exit(EXIT_SUCCESS);
}

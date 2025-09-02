/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:27:04 by llage             #+#    #+#             */
/*   Updated: 2025/08/31 21:27:47 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*create_prompt(t_map env)
{
	const char	*nametag = ANSI_RED "[" SHELL_NAME "]" ANSI_GREEN;
	const char	*user = map_get(&env, "USER");
	const char	*hostname = map_get(&env, "NAME");
	const char	*cwd = getcwd(NULL, 0);
	char		*prompt;

	if (user != NULL)
		prompt = ft_strjoin(nametag, user);
	else
		prompt = ft_strjoin(nametag, "guest");
	if (hostname != NULL)
	{
		prompt = ft_str_add(prompt, "@");
		prompt = ft_str_add(prompt, hostname);
	}
	prompt = ft_str_add(prompt, ANSI_RESET ":" ANSI_BLUE);
	prompt = ft_str_add(prompt, cwd);
	free((void *) cwd);
	prompt = ft_str_add(prompt, ANSI_RESET "$ ");
	return (prompt);
}

static t_shell	create_shell(char *envp[])
{
	t_shell	shell;

	shell.exit_status = 0;
	shell.env = create_env(envp);
	if (shell.env.size == (size_t) -1)
	{
		perror(SHELL_NAME);
		exit(EXIT_FAILURE);
	}
	return (shell);
}

// TODO: Signals
int	main(int argc, char *argv[], char *envp[])
{
	const t_cmd_table	*cmd_table;
	t_token				*tokens;
	char				*prompt;
	char				*line;
	t_shell				shell;

	(void)argc;
	(void)argv;
	shell = create_shell(envp);
	while (true)
	{
		prompt = create_prompt(shell.env);
		// TODO: Set the signal handlers
		line = readline(prompt);
		free(prompt);
		if (line == NULL)
			break ;
		if (*skip_whitespace(line) == '\0')
		{
			free(line);
			continue ;
		}
		if (line[0] != ' ')
			add_history(line);
		tokens = lexer(line);
		free(line);
		if (tokens != NULL)
		{
			if (DEBUG)
				print_tokens(*tokens);
			cmd_table = parser(&tokens, &shell);
			free_tokens(&tokens);
			if (cmd_table != NULL)
			{
				if (DEBUG)
					print_cmd_table(cmd_table);
				// TODO: Remove the signal handlers meow
				exec_table(cmd_table, &shell);
				free_cmd_table((t_cmd_table **) &cmd_table);
			}
		}
	}
	map_free(&shell.env);
	exit(shell.exit_status);
}

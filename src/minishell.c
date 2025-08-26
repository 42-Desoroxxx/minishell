/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:27:04 by llage             #+#    #+#             */
/*   Updated: 2025/08/26 06:55:15 by llage            ###   ########.fr       */
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

// TODO: Signals
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
	while (true)
	{
		prompt = create_prompt(env);
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
				exec_table((t_cmd *) cmd_table, env);
				free_cmd_table((t_cmd **) &cmd_table);
			}
		}
	}
	map_free((t_map *) &env);
	exit(EXIT_SUCCESS);
}

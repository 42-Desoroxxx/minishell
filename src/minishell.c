/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:27:04 by llage             #+#    #+#             */
/*   Updated: 2025/08/17 05:09:27 by logname          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_data.h"
#include <minishell.h>
#include <stdlib.h>

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

// TODO: be not shit
// TODO: set the shelllvl +1
static t_map	create_env(char *envp[])
{
	t_map	env;
	char	*cur;
	char	*key;
	char	*value;
	int		key_len;
	int		i;

	env = map_bzero();
	i = 0;
	while (envp[i])
	{
		cur = envp[i];
		key_len = 0;
		key = NULL;
		value = NULL;
		while (cur[key_len] && cur[key_len] != '=')
			key_len++;
		key = ft_strndup(cur, key_len);
		if (key == NULL)
		{
			map_free(&env);
			env.size = -1;
			return (env);
		}
		if (!map_set(&env, key, &(cur[key_len + 1])))
		{
			free(key);
			map_free(&env);
			env.size = -1;
			return (env);
		}
		free(key);
		i++;
	}
	return (env);
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
	if (DEBUG)
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
			continue ;
		tokens = lexer(line, env);
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
	map_free((t_map *) &env);
	exit(EXIT_SUCCESS);
}

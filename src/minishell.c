/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:27:04 by llage             #+#    #+#             */
/*   Updated: 2025/09/05 16:44:59 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static volatile sig_atomic_t	g_signal = 0;

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

static void	handle_interupt(const int signal)
{
	if (signal != SIGINT)
		return ;
	g_signal = SIGINT;
	rl_crlf();
	rl_replace_line("", 0);
	rl_set_prompt(ANSI_RED "> " ANSI_RESET);
	rl_expand_prompt(rl_prompt);
	rl_on_new_line();
	rl_redisplay();
}

int	main(int argc, char *argv[], char *envp[])
{
	t_cmd_table	*cmd_table;
	t_token		*tokens;
	char		*prompt;
	char		*line;
	t_shell		shell;

	(void)argc;
	(void)argv;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_interupt);
	shell = create_shell(envp);
	while (true)
	{
		prompt = get_prompt(&shell);
		line = readline(prompt);
		if (g_signal == SIGINT)
			shell.exit_status = 130;
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
			if (cmd_table == NULL)
			{
				if (shell.exit_status != 2)
					perror(SHELL_NAME);
				free_cmd_table((t_cmd_table **) &cmd_table);
				continue ;
			}
			if (cmd_table != NULL)
			{
				if (DEBUG)
					print_cmd_table(cmd_table);
				exec_table(cmd_table, &shell);
				free_cmd_table((t_cmd_table **) &cmd_table);
			}
		}
	}
	map_free(&shell.env);
	exit(shell.exit_status);
}

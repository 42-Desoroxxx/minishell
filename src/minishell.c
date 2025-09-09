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
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static void	process(char *line, t_shell *shell)
{
	t_cmd_table	*cmd_table;
	t_token		*tokens;

	if (*skip_whitespace(line) == '\0')
	{
		free(line);
		return ;
	}
	add_history(line);
	tokens = lexer(line);
	free(line);
	if (tokens == NULL)
		return ;
	cmd_table = parser(&tokens, shell);
	free_tokens(&tokens);
	if (cmd_table == NULL)
	{
		if (shell->exit_status != 2)
			perror(SHELL_NAME);
		free_cmd_table((t_cmd_table **) &cmd_table);
		return ;
	}
	exec_table(cmd_table, shell);
	free_cmd_table((t_cmd_table **) &cmd_table);
}

static void	setup_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_interupt);
}

int	main(int argc, char *argv[], char *envp[])
{
	char		*prompt;
	char		*line;
	t_shell		shell;

	(void)argc;
	(void)argv;
	setup_signals();
	shell = create_shell(envp);
	while (true)
	{
		prompt = get_prompt(&shell);
		line = readline(prompt);
		free(prompt);
		if (g_signal == SIGINT)
		{
			shell.exit_status = 130;
			g_signal = 0;
		}
		if (line == NULL)
			break ;
		process(line, &shell);
	}
	rl_clear_history();
	map_free(&shell.env);
	exit(shell.exit_status);
}

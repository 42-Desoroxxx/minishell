/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 01:18:01 by llage             #+#    #+#             */
/*   Updated: 2025/09/08 01:18:01 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*add_exit_status(const t_shell *shell, char *prompt)
{
	if (shell->exit_status == 0)
		return (prompt);
	prompt = ft_str_add(prompt, ANSI_RED " [");
	prompt = ft_str_add(prompt, ft_byte_to_str(shell->exit_status));
	prompt = ft_str_add(prompt, "]" ANSI_RED);
	return (prompt);
}

static char	*add_current_dir(const t_shell *shell, char *prompt)
{
	const char		*cwd = getcwd(NULL, 0);
	const size_t	cwd_len = ft_strlen(cwd);
	const char		*home = map_get(&shell->env, "HOME");
	const size_t	home_len = ft_strlen(home);
	char			*tmp;

	if (cwd == NULL)
		return (prompt);
	prompt = ft_str_add(prompt, " " ANSI_YELLOW);
	if (home != NULL && home_len <= cwd_len
		&& ft_strncmp(cwd, home, ft_strlen(home)) == 0
		&& (cwd[home_len] == '/' || cwd[home_len] == '\0'))
	{
		prompt = ft_str_add(prompt, "~");
		tmp = ft_strdup(cwd + home_len);
		free((void *) cwd);
		cwd = tmp;
	}
	prompt = ft_str_add(prompt, cwd);
	if (cwd != NULL)
		free((void *) cwd);
	return (prompt);
}

static char	*create_prompt(const t_shell *shell)
{
	const char	*at = ANSI_RESET "@" ANSI_BLUE;
	const char	*user = map_get(&shell->env, "USER");
	const char	*hostname = map_get(&shell->env, "NAME");
	char		*prompt;

	if (user != NULL)
	{
		prompt = ft_strjoin(ANSI_CYAN, user);
		prompt = ft_str_add(prompt, at);
	}
	else
		prompt = ft_strjoin(ANSI_CYAN "guest", at);
	if (hostname != NULL)
		prompt = ft_str_add(prompt, hostname);
	else
		prompt = ft_str_add(prompt, "mystery");
	prompt = add_current_dir(shell, prompt);
	prompt = add_exit_status(shell, prompt);
	return (prompt);
}

char	*get_prompt(const t_shell *shell)
{
	char	*prompt;

	prompt = create_prompt(shell);
	if (prompt == NULL)
		return (NULL);
	if (shell->exit_status != 0)
		prompt = ft_str_add(prompt, "\n" ANSI_RED "> " ANSI_RESET);
	else
		prompt = ft_str_add(prompt, "\n" ANSI_GREEN "> " ANSI_RESET);
	return (prompt);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:02:48 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/31 23:13:52 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

#define RANDOM_STRING_LEN 128

static char	*random_string(void)
{
	static unsigned char	string[RANDOM_STRING_LEN + 1];
	int						fd;
	int						i;

	bzero(string, RANDOM_STRING_LEN + 1);
	fd = open("/dev/urandom", O_RDONLY | O_CLOEXEC);
	read(fd, string, RANDOM_STRING_LEN);
	i = 0;
	string[i] = '.';
	while (++i < RANDOM_STRING_LEN)
		string[i] = (string[i] % 26) + 'a';
	close(fd);
	return ((char *) string);
}

static void	read_heredoc_input(int fd, char *delimiter, bool expand,
	t_shell *shell)
{
	char		*line;
	int			line_count;

	line_count = 0;
	while (true)
	{
		line = readline("> ");
		if (line == NULL)
			ft_fprintf(STDERR_FILENO, "\n" ANSI_YELLOW SHELL_NAME
				" [WARNING]: Here-document at line %d delimited by end-of-file"
				" (wanted `%s`)\n" ANSI_RESET, line_count + 1, delimiter);
		if (line == NULL)
			break ;
		if (!ft_str_equal(line, delimiter))
		{
			line_count++;
			if (expand)
				line = expand_str(line, shell);
			ft_fprintf(fd, "%s\n", line);
		}
		else
			break ;
		free(line);
	}
	free(line);
}

static bool	should_expand(char *delimiter)
{
	return (ft_strchr(delimiter, '\'') == 0
		&& ft_strchr(delimiter, '"') == 0);
}

int	parse_heredoc(t_token *token, bool last, t_shell *shell)
{
	char	rnd_filename[5 + RANDOM_STRING_LEN + 1];
	char	*delimiter;
	int		fd;

	delimiter = remove_closed_quotes(token->value);
	if (delimiter == NULL)
		return (-1);
	ft_strlcat(rnd_filename, "/tmp/", 5 + RANDOM_STRING_LEN + 1);
	ft_strlcat(rnd_filename, random_string(), 5 + RANDOM_STRING_LEN + 1);
	fd = open(rnd_filename, O_CREAT | O_WRONLY | O_CLOEXEC | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	ft_printf(SHELL_NAME ": Here-document, waiting for `%s`\n", delimiter);
	read_heredoc_input(fd, delimiter, should_expand(token->value), shell);
	free(delimiter);
	close(fd);
	if (last)
	{
		fd = open(rnd_filename, O_RDONLY | O_CLOEXEC);
		if (fd < 0)
			return (-1);
		return (fd);
	}
	unlink(rnd_filename);
	return (-2);
}

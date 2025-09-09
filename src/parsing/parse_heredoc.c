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

	ft_bzero(string, RANDOM_STRING_LEN + 1);
	fd = open("/dev/urandom", O_RDONLY | O_CLOEXEC);
	if (fd < 0)
	{
		perror(SHELL_NAME);
		return (NULL);
	}
	if (read(fd, string, RANDOM_STRING_LEN) < 0)
		close(close(fd));
	i = 0;
	string[i] = '.';
	while (++i < RANDOM_STRING_LEN)
		string[i] = (string[i] % 26) + 'a';
	close(fd);
	return ((char *) string);
}

static void	print_error(const int line_count, const char *delimiter)
{
	ft_fprintf(STDERR_FILENO, "\n" ANSI_YELLOW SHELL_NAME
		" [WARNING]: Here-document at line %d delimited by end-of-file"
		" (wanted `%s`)\n" ANSI_RESET, line_count + 1, delimiter);
}

static void	read_heredoc_input(int fd, char *delimiter, bool expand,
	t_shell *shell)
{
	char		*line;
	char		*tmp;
	int			line_count;

	line_count = 0;
	while (true)
	{
		line = readline("> ");
		if (line == NULL)
			print_error(line_count, delimiter);
		if (line == NULL || ft_str_equal(line, delimiter))
			break ;
		line_count++;
		if (expand)
		{
			tmp = expand_str(line, shell);
			if (tmp == NULL)
				break ;
			free(line);
			line = tmp;
		}
		ft_fprintf(fd, "%s\n", line);
		free(line);
	}
	free(line);
}

static bool	contains_quotes(char *delimiter)
{
	return (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '"'));
}

int	parse_heredoc(t_token *token, bool last, t_shell *shell,
	const int previous_fd)
{
	char	rnd_filename[5 + RANDOM_STRING_LEN + 1];
	char	*delimiter;
	int		fd;

	delimiter = remove_closed_quotes(token->value);
	if (delimiter == NULL)
		return (-1);
	ft_bzero(rnd_filename, 5 + RANDOM_STRING_LEN + 1);
	ft_strlcat(rnd_filename, "/tmp/", 5 + RANDOM_STRING_LEN + 1);
	ft_strlcat(rnd_filename, random_string(), 5 + RANDOM_STRING_LEN + 1);
	fd = open(rnd_filename, O_CREAT | O_WRONLY | O_CLOEXEC | O_TRUNC, 0644);
	if (fd < 0)
		perror(SHELL_NAME);
	if (fd < 0)
		return (-1);
	ft_printf(SHELL_NAME ": Here-document, waiting for `%s`\n", delimiter);
	read_heredoc_input(fd, delimiter, !contains_quotes(token->value), shell);
	free(delimiter);
	close(fd);
	if (last && previous_fd != -1)
		return (open(rnd_filename, O_RDONLY | O_CLOEXEC));
	unlink(rnd_filename);
	if (previous_fd == -1)
		return (-1);
	return (-2);
}

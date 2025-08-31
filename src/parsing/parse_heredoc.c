/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:02:48 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/31 21:41:41 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*random_string(void)
{
	static unsigned char	string[17 + 1] = {0};
	int						fd;
	int						i;

	fd = open("/dev/urandom", O_RDONLY);
	read(fd, string, 16);
	i = 0;
	string[i] = '.';
	while (++i < 16)
		string[i] = (string[i] % 26) + 'a';
	close(fd);
	return ((char *) string);
}

static void	read_heredoc_input(int fd, t_token token, t_shell shell)
{
	char	*line;
	int		line_count;

	line_count = 0;
	while (true)
	{
		line = readline("> ");
		if (line == NULL)
		{
			ft_fprintf(STDERR_FILENO, "\n" ANSI_YELLOW SHELL_NAME
				" [WARNING]: Here-document at line %d delimited by end-of-file"
				" (wanted `%s`)\n" ANSI_RESET, line_count + 1, token.value);
			break ;
		}
		if (!ft_str_equal(line, token.value))
		{
			line_count++;
			line = expand_line(line, shell);
			ft_fprintf(fd, line);
			ft_fprintf(fd, "\n");
		}
		else
			break ;
		free(line);
	}
	free(line);
}

static bool	has_quote(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == '\'')
			return (true);
		i++;
	}
	return (false);
}

int	parse_heredoc(t_token token, bool last, t_shell shell)
{
	char	*rnd_filename;
	int		fd;

	rnd_filename = ft_strjoin("/tmp/", random_string());
	fd = open(rnd_filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
	{
		free(rnd_filename);
		return (-2);
	}
	if (!has_quote(token.value))
		read_heredoc_input(fd, token, shell);
	close(fd);
	fd = open(rnd_filename, O_RDONLY);
	free(rnd_filename);
	if (!last)
	{
		close(fd);
		return (0);
	}
	return (fd);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvitiell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:02:48 by rvitiell          #+#    #+#             */
/*   Updated: 2025/08/25 07:02:51 by rvitiell         ###   ########.fr       */
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

// TODO: CTRL + D in heredoc simply does not close it
static void	read_heredoc_input(int fd, t_token token, const t_map env, int status)
{
	char	*line;

	while (true)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		line = expand_line(line, env, status);
		if (!ft_str_equal(line, token.value))
		{
			ft_fprintf(fd, line);
			ft_fprintf(fd, "\n");
		}
		else
			break ;
		free(line);
	}
	free(line);
}

bool	has_quote(char *value)
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

int	parse_heredoc(t_token token, bool last, const t_map env, int status)
{
	char	*rnd_filename;
	int		fd;

	rnd_filename = ft_strjoin("/tmp/", random_string());
	fd = open((char *)rnd_filename,
			O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(rnd_filename);
	if (fd == 0)
		return (-2);
	if (!has_quote(token.value))
		read_heredoc_input(fd, token, env, status);
	if (!last)
	{
		close(fd);
		return (0);
	}
	return (fd);
}

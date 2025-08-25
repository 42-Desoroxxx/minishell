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

static unsigned char	*random_string(void)
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
	return (string);
}

static void	read_heredoc_input(int fd, t_token token, const t_map env)
{
	char	*line;

	while (true)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			continue ;
		line = expand_line(line, env);
		if (ft_str_equal(line, token.value) != 0)
			ft_fprintf(fd, line);
		else
			break ;
		free(line);
	}
	free(line);
}

int	parse_heredoc(t_token token, bool last, const t_map env)
{
	unsigned char		*rnd_filename;
	int					fd;

	rnd_filename = random_string();
	fd = open((char *)rnd_filename,
			O_CREAT | O_TRUNC | S_IRWXU | O_APPEND, __O_TMPFILE, 0644);
	if (fd == 0)
		return (-2);
	read_heredoc_input(fd, token, env);
	if (!last)
	{
		close(fd);
		return (0);
	}
	return (fd);
}

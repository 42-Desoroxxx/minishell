/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llage <llage@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 18:30:00 by llage             #+#    #+#             */
/*   Updated: 2025/08/12 19:25:00 by llage            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_close_fd_array(int **array_ptr)
{
	int		*array;
	size_t	i;

	array = *array_ptr;
	if (array_ptr == NULL || array == NULL)
		return ;
	i = 0;
	while (array[i] != -1)
		close(array[i++]);
	free(array);
	*array_ptr = NULL;
}

void	free_str_array(char ***array_ptr)
{
	char	**array;
	size_t	i;

	array = *array_ptr;
	if (array_ptr == NULL || array == NULL)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
	array = NULL;
}

void	free_tokens(t_token **token_ptr)
{
	t_token	*token;

	token = *token_ptr;
	if (token_ptr == NULL || token == NULL)
		return ;
	while (token->prev)
		token = token->prev;
	while (token->next)
	{
		token = token->next;
		free(token->prev);
	}
	free(token);
	*token_ptr = NULL;
}

void	free_cmd_table(t_cmd **cmd_table_ptr)
{
	t_cmd	*cmd_table;
	size_t	i;

	cmd_table = *cmd_table_ptr;
	if (cmd_table_ptr == NULL || cmd_table == NULL)
		return ;
	i = 0;
	while (cmd_table[i].args || cmd_table[i].in_redir || cmd_table[i].out_redir)
	{
		free_str_array(&cmd_table[i].args);
		free_close_fd_array(&cmd_table[i].in_redir);
		free_close_fd_array(&cmd_table[i].out_redir);
		i++;
	}
	free(cmd_table);
	*cmd_table_ptr = NULL;
}

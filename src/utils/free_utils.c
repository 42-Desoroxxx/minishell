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
		if (token->prev->value != NULL)
		{
			free(token->prev->value);
			token->prev->value = NULL;
		}
		free(token->prev);
		token->prev = NULL;
	}
	free(token);
	*token_ptr = NULL;
}

void	free_cmd_table(t_cmd_table **cmd_table_ptr)
{
	t_cmd_table	*cmd_table;
	size_t		i;

	cmd_table = *cmd_table_ptr;
	if (cmd_table_ptr == NULL || cmd_table == NULL)
		return ;
	i = -1;
	while (++i < cmd_table->size)
	{
		free_str_array(&cmd_table->cmds[i].args);
		if (cmd_table->cmds[i].in_redir > 0)
			close(cmd_table->cmds[i].in_redir);
		if (cmd_table->cmds[i].out_redir > 0)
			close(cmd_table->cmds[i].out_redir);
	}
	free(cmd_table);
	*cmd_table_ptr = NULL;
}

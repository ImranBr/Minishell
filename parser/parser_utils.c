/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 20:56:02 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/04 20:26:53 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast_node	*create_ast_node(t_node_type node_type)
{
	t_ast_node	*new_node;

	new_node = malloc(sizeof(t_ast_node));
	if (!new_node)
		return (NULL);
	new_node->type = node_type;
	new_node->args = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->filename = NULL;
	return (new_node);
}

int	add_args_to_cmd(t_ast_node *cmd, char *arg)
{
	char	**tmp;
	int		i;

	i = 0;
	tmp = malloc(sizeof(char *) * (cmd->args_count + 2));
	if (!tmp)
		return (0);
	while (i < cmd->args_count)
	{
		tmp[i] = cmd->args[i];
		i++;
	}
	tmp[cmd->args_count] = arg;
	tmp[cmd->args_count + 1] = NULL;
	free(cmd->args);
	cmd->args = tmp;
	cmd->args_count++;
	return (1);
}

void	add_redir_to_cmd(t_ast_node *cmd, t_ast_node *redir)
{
	t_ast_node	*current;

	if (!cmd || !redir)
		return ;
	if (!cmd->right)
		cmd->right = redir;
	else
	{
		current = cmd->right;
		while (current->right)
			current = current->right;
		current->right = redir;
	}
}

void	free_ast(t_ast_node *node)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	if (node->type == NODE_COMMAND && node->args)
	{
		while (i < node->args_count)
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	if (node->filename)
		free(node->filename);
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

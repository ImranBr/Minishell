/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:33:48 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/10 23:31:06 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast_node	*create_redir_node(t_ast_node *cmd, t_token *token_list)
{
	t_ast_node		*redir;
	t_token_type	redir_type;

	redir = NULL;
	redir_type = (token_list)->type;
	token_list = (token_list)->next;
	if (!token_list || (token_list)->type != WORD)
	{
		write(STDERR_FILENO, "syntax error\n", 13);
		free_ast(cmd);
		return (NULL);
	}
	redir = create_ast_node(NODE_REDIRECT);
	redir->redir_type = redir_type;
	redir->filename = ft_strdup((token_list)->value);
	return (redir);
}

t_ast_node	*parse_command(t_token **token_list)
{
	t_ast_node	*cmd;
	t_ast_node	*redir;

	cmd = NULL;
	if (!token_list || !*token_list)
		return (NULL);
	while (*token_list && (*token_list)->type != PIPE)
	{
		if ((*token_list)->type == WORD)
		{
			if (!cmd)
				cmd = create_ast_node(NODE_COMMAND);
			add_args_to_cmd(cmd, ft_strdup((*token_list)->value));
		}
		else if ((*token_list)->type == REDIRECT_IN
			|| (*token_list)->type == REDIRECT_OUT
			|| (*token_list)->type == APPEND || (*token_list)->type == HEREDOC)
		{
			if (!cmd)
				cmd = create_ast_node(NODE_COMMAND);
			add_args_to_cmd(cmd, ft_strdup((*token_list)->value));
			redir = create_redir_node(cmd, *token_list);
			add_redir_to_cmd(cmd, redir);
			*token_list = (*token_list)->next;
		}
		*token_list = (*token_list)->next;
	}
	return (cmd);
}

t_ast_node	*parse_pipeline(t_token **token_list)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*node_pipe;

	if (*token_list && (*token_list)->type == PIPE)
	{
		write(STDERR_FILENO, "syntax error near unexpected token `|'\n", 39);
		return (NULL);
	}
	left = parse_command(token_list);
	if (!left)
		return (NULL);
	while (*token_list && (*token_list)->type == PIPE)
	{
		*token_list = (*token_list)->next;
		right = parse_command(token_list);
		if (!right)
		{
			write(STDERR_FILENO, "syntax error\n", 13);
			free_ast(left);
			return (NULL);
		}
		node_pipe = create_ast_node(NODE_PIPE);
		node_pipe->left = left;
		node_pipe->right = right;
		left = node_pipe;
	}
	return (left);
}

// Implémentation de ft_strdup (si pas déjà incluse)
char	*ft_strdup(const char *s)
{
	int		len;
	char	*copy;

	len = strlen(s) + 1;
	copy = malloc(len);
	if (copy)
		memcpy(copy, s, len);
	return (copy);
}

void	print_indent(int level, bool is_last, bool *branches)
{
	for (int i = 0; i < level; i++)
	{
		if (branches[i])
			printf("│   ");
		else
			printf("    ");
	}
	if (is_last)
		printf("└── ");
	else
		printf("├── ");
}

void	print_ast(t_ast_node *node, int level, bool is_last, bool *branches)
{
	t_ast_node	*redir;
	int			redir_index;
	bool		last_redir;

	if (!node)
		return ;
	print_indent(level, is_last, branches);
	if (node->type == NODE_PIPE)
	{
		printf("PIPE\n");
		branches[level] = !is_last;
			// Marque qu'on continue à dessiner la colonne si ce n'est pas le dernier enfant
		print_ast(node->left, level + 1, false, branches);
		print_ast(node->right, level + 1, true, branches);
	}
	else if (node->type == NODE_COMMAND)
	{
		printf("COMMAND\n");
		for (int i = 0; i < node->args_count; i++)
		{
			print_indent(level + 1, i == node->args_count - 1
				&& node->right == NULL, branches);
			printf("arg[%d]: %s\n", i, node->args[i]);
		}
		redir = node->right;
		redir_index = 0;
		while (redir)
		{
			last_redir = (redir->right == NULL);
			print_indent(level + 1, last_redir, branches);
			printf("REDIRECT (%d): %s\n", redir->type, redir->filename);
			redir = redir->right;
			redir_index++;
		}
	}
}


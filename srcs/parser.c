/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:33:48 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/11 20:58:26 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
	if (!cmd->redirections)
		cmd->redirections = redir;
	else
	{
		current = cmd->redirections;
		while (current->right)
			current = current->right;
		current->right = redir;
	}
}

t_ast_node	*parse_command(t_token **token_list, t_ast_node *cmd)
{
	t_ast_node	*redir;

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
		else if ((*token_list)->type == 2 || (*token_list)->type == 3
			|| (*token_list)->type == 4 || (*token_list)->type == 5)
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

t_ast_node	*handle_pipe_sequence(t_token **token_list, t_ast_node *left)
{
	t_ast_node	*right;
	t_ast_node	*node_pipe;

	*token_list = (*token_list)->next;
	right = parse_command(token_list, NULL);
	if (!right)
	{
		write(STDERR_FILENO, "syntax error\n", 13);
		free_ast(left);
		return (NULL);
	}
	node_pipe = create_ast_node(NODE_PIPE);
	node_pipe->left = left;
	node_pipe->right = right;
	return (node_pipe);
}

t_ast_node	*parse_pipeline(t_token **token_list)
{
	t_ast_node	*left;

	if (*token_list && (*token_list)->type == PIPE)
	{
		write(STDERR_FILENO, "syntax error near unexpected token `|'\n", 39);
		return (NULL);
	}
	left = parse_command(token_list, NULL);
	if (!left)
		return (NULL);
	while (*token_list && (*token_list)->type == PIPE)
	{
		left = handle_pipe_sequence(token_list, left);
		if (!left)
			return (NULL);
	}
	return (left);
}

/*
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
	left = parse_command(token_list, NULL);
	if (!left)
		return (NULL);
	while (*token_list && (*token_list)->type == PIPE)
	{
		*token_list = (*token_list)->next;
		right = parse_command(token_list, NULL);
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
}*/
/*
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
*/

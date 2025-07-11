/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 20:56:02 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/11 13:03:03 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_ast_node	*create_ast_node(t_node_type node_type)
{
	t_ast_node	*new_node;

	new_node = malloc(sizeof(t_ast_node));
	if (!new_node)
		return (NULL);
	new_node->type = node_type;
	new_node->args = NULL;
	new_node->args_count = 0;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->redirections = NULL;
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

// void	add_redir_to_cmd(t_ast_node *cmd, t_ast_node *redir)
// {
// 	t_ast_node	*current;

// 	if (!cmd || !redir)
// 		return ;
// 	if (!cmd->right)
// 		cmd->right = redir;
// 	else
// 	{
// 		current = cmd->right;
// 		while (current->right)
// 			current = current->right;
// 		current->right = redir;
// 	}
// }

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

void	process_heredoc(t_ast_node *heredoc_node)
{
	int		fd_input;
	char	*line;

	// char	tmp[] = "/tmp/herdocxx";
	if (!heredoc_node || !heredoc_node->filename)
	{
		fprintf(stderr, "Erreur: heredoc_node ou son filename est NULL\n");
		return ;
	}
	fd_input = open("input.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_input == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || strcmp(line, heredoc_node->filename) == 0)
		{
			free(line);
			break ;
		}
		if (write(fd_input, line, strlen(line)) == -1 || write(fd_input, "\n",
				1) == -1)
		{
			perror("write");
			free(line);
			close(fd_input);
			exit(EXIT_FAILURE);
		}
	}
	close(fd_input);
}

void	process_redirect_in(t_ast_node *redirect_in_node)
{
	int	fd_input;

	fd_input = open(redirect_in_node->filename, O_RDONLY);
	if (fd_input == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd_input, 0) == -1)
	{
		perror("dup2");
		close(fd_input);
		exit(EXIT_FAILURE);
	}
	close(fd_input);
	// printf("redirect_in -> %s\n", redirect_in_node->filename);
}

// void	process_redirect_out(t_ast_node *redirect_out_node)
// {
// 	int	fd_output;

// 	fd_output = open(redirect_out_node->filename, O_WRONLY | O_CREAT | O_TRUNC,
// 			0644);
// 	if (fd_output == -1)
// 	{
// 		perror("open");
// 		exit(EXIT_FAILURE);
// 	}
// 	dup2(fd_output, 1);
// 	// printf("redirect_out -> %s\n", redirect_out_node->filename);
// 	close(fd_output);
// }
int	process_redirect_out(t_ast_node *redirect_out_node)
{
	int	fd_output;

	fd_output = open(redirect_out_node->filename, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (fd_output == -1)
	{
		perror(redirect_out_node->filename);
		return (-1);
	}
	if (dup2(fd_output, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd_output);
		return (-1);
	}
	close(fd_output);
	return (0);
}

void	process_append(t_ast_node *append_node)
{
	int	fd_output;

	fd_output = open(append_node->filename, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	if (fd_output == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd_output, 1);
	// printf("append -> %s\n", append_node->filename);
	close(fd_output);
}

void	*process_redirections(t_ast_node *redir_list)
{
	t_ast_node	*current_node;

	current_node = redir_list;
	while (current_node)
	{
		if (current_node->redir_type == HEREDOC)
		{
			process_heredoc(current_node);
			process_redirect_in(current_node);
		}
		else if (current_node->redir_type == REDIRECT_IN)
		{
			process_redirect_in(current_node);
		}
		else if (current_node->redir_type == REDIRECT_OUT)
			process_redirect_out(current_node);
		else if (current_node->redir_type == APPEND)
			process_append(current_node);
		current_node = current_node->right;
	}
	return (NULL);
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

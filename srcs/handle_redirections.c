/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:59:41 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/11 21:54:33 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

void	read_heredoc_lines(int fd_input, const char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (write(fd_input, line, strlen(line)) == -1
			|| write(fd_input, "\n", 1) == -1)
		{
			perror("write");
			free(line);
			close(fd_input);
			exit(EXIT_FAILURE);
		}
		free(line);
	}
}

void	process_heredoc(t_ast_node *heredoc_node, const char *outfile)
{
	int	fd_input;

	if (!heredoc_node || !heredoc_node->filename)
	{
		fprintf(stderr, "Erreur: heredoc_node ou son filename est NULL\n");
		return ;
	}
	fd_input = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_input == -1)
	{
		perror(outfile);
		exit(EXIT_FAILURE);
	}
	read_heredoc_lines(fd_input, heredoc_node->filename);
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
}

/*void	process_redirect_out(t_ast_node *redirect_out_node)
{
	int	fd_output;

	fd_output = open(redirect_out_node->filename, O_WRONLY | O_CREAT | O_TRUNC,
			0644);
	if (fd_output == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd_output, 1);
	// printf("redirect_out -> %s\n", redirect_out_node->filename);
	close(fd_output);
}*/
int	process_redirect_out(t_ast_node *redirect_out_node)
{
	int	fd_output;

	fd_output = open(redirect_out_node->filename, O_WRONLY
			| O_CREAT | O_TRUNC, 0644);
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
			process_heredoc(current_node, "heredoc");
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

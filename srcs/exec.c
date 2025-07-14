/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:15:42 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/14 17:16:28 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int	save_stdout(void)
{
	int saved_fd = dup(STDOUT_FILENO);
	if (saved_fd == -1)
		perror("dup");
	return saved_fd;
}

void	restore_stdout(int saved_fd)
{
	if (dup2(saved_fd, STDOUT_FILENO) == -1)
		perror("dup2 restore");
	close(saved_fd);
}

void free_in_child(t_env *env_list, t_ast_node *node, char *path)
{
	free_list(env_list);
	free_ast(node);
	if (path)
		free(path);
}

char **get_path(t_env *env_list)
{
	char *path_name;
	char **tab;

	path_name = "PATH";
	tab = NULL;
	while (env_list)
	{
		if (env_list->name && ft_strcmp(env_list->name, path_name) == 0)
		{
			if (env_list->value)
				tab = ft_split(env_list->value, ':');
			return (tab);
		}
		env_list = env_list->next;
	}
	return (NULL);
}

char *get_cmd(t_env *env_list, char *s1)
{
	int i;
	char **str;
	char *pathname;

	i = 0;
	if (!s1 || s1[0] == '\0')
		return (ft_putendl_fd("Command not found", STDERR_FILENO), NULL);
	if (s1[0] == '/')
	{
		if (access(s1, F_OK | X_OK) == 0)
			return (ft_strdup(s1));
		return (perror(s1), NULL);
	}
	str = get_path(env_list);
	if (!str)
		return (perror("PATH not found"), NULL);
	while (str[i])
	{
		pathname = ft_strjoin2(str[i], s1);
		if (pathname && access(pathname, F_OK | X_OK) == 0)
			return (free_split(str), pathname);
		free(pathname);
		i++;
	}
	return (free_split(str), perror(s1), NULL);
}

int is_builtin(char *cmd)
{
	return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "exit") == 0);
}

int needs_child_process(char *cmd)
{
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "env") == 0);
}

int execute_ast(t_ast_node *node, char **envp, t_env *env_list, t_token *token,
				char *input)
{
	pid_t pid_left;
	pid_t pid_right;
	pid_t pid_cmd;
	int pipe_fd[2];
	char *path;

	path = NULL;
	if (node->type == NODE_PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (EXIT_FAILURE);
		}
		pid_left = fork();
		if (pid_left == -1)
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		if (pid_left == 0)
		{
			signal(SIGQUIT, SIG_DFL);
			signal(SIGINT, SIG_DFL);
			close(pipe_fd[0]);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
			execute_ast(node->left, envp, env_list, token, input);
			free_in_child(env_list, node, path);
			exit(EXIT_FAILURE);
		}
		pid_right = fork();
		if (pid_right == -1)
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (pid_right == 0)
		{
			close(pipe_fd[1]);
			dup2(pipe_fd[0], STDIN_FILENO);
			close(pipe_fd[0]);
			execute_ast(node->right, envp, env_list, token, input);
			free_in_child(env_list, node, path);
			free(input);
			free_tokens(token);
			exit(EXIT_FAILURE);
		}
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		waitpid(pid_left, NULL, 0);
		waitpid(pid_right, NULL, 0);
	}
	else if (node->type == NODE_COMMAND)
	{
		int saved_stdout = -1;
		if ((!node->args || !node->args[0]) && node->redirections)
		{
			saved_stdout = save_stdout();
			process_redirections(node->redirections);
			restore_stdout(saved_stdout);
			return (0);
		}

		if (node->args && node->args[0] && is_builtin(node->args[0]) && !needs_child_process(node->args[0]))
		{
			exec_builtin(node->args, env_list, NULL);
			return (0);
		}
		pid_cmd = fork();
		if (pid_cmd == -1)
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (pid_cmd == 0)
		{
			process_redirections(node->redirections);
			// {
			// 	perror("redirection");
			// 	exit(EXIT_FAILURE);
			// }
			if (node->args && node->args[0] && is_builtin(node->args[0]) && needs_child_process(node->args[0]))
			{
				exec_builtin(node->args, env_list, NULL);
				free_in_child(env_list, node, path);
				free_tokens(token);
				free(input);
				exit(0);
			}
			path = get_cmd(env_list, node->args[0]);
			if (!path)
			{
				perror("command not found");
				free_in_child(env_list, node, path);
				free_tokens(token);
				free(input);
				exit(127);
			}
			execve(path, node->args, envp);
			free_in_child(env_list, node, path);
			perror("execve");
			free_tokens(token);
			free(input);
			exit(EXIT_FAILURE);
		}
		waitpid(pid_cmd, NULL, 0);
		unlink("heredoc");
	}
	return (0);
}

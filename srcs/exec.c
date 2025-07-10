/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:15:42 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/10 16:42:33 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

char	**get_path(t_env *env_list)
{
	char	*path;
	char	**tab;
	// int		i;

	if (!env_list->env)
		return (NULL);
	tab = NULL;
	// i = 0;
	path = "PATH=";
	while (env_list->env && env_list->env)
	{
		if (ft_strncmp( env_list->env, path, 5) == 0)
		{
			tab = ft_split(env_list->env + (char) 5, ':');
			return (tab);
		}
		env_list->env++;
	}
	return (NULL);
}

char	*get_cmd(t_env *env_list, char *s1)
{
	int		i;
	char	**str;
	char	*pathname;

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

int	is_builtin(char *cmd)
{
	return (ft_strcmp(cmd, "cd") == 0
        || ft_strcmp(cmd, "echo") == 0
        || ft_strcmp(cmd, "pwd") == 0
        || ft_strcmp(cmd, "env") == 0
        || ft_strcmp(cmd, "export") == 0
        || ft_strcmp(cmd, "unset") == 0
        || ft_strcmp(cmd, "exit") == 0);
}

int	needs_child_process(char *cmd)
{
	return (ft_strcmp(cmd, "echo") == 0
        || ft_strcmp(cmd, "pwd") == 0
        || ft_strcmp(cmd, "env") == 0);
}


int	execute_ast(t_ast_node *node, char **envp, t_env *env_list)
{
	pid_t	pid_left;
	pid_t	pid_right;
	pid_t	pid_cmd;
	int		pipe_fd[2];
	char	**path;

	if (node->type == NODE_PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return(EXIT_FAILURE);
		}
		if ((pid_left = fork()) == -1)
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		if (pid_left == 0)
		{
			close(pipe_fd[0]);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
			execute_ast(node->left, envp, env_list);
			exit(EXIT_FAILURE);
		}
		if ((pid_right = fork()) == -1)
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		if (pid_right == 0)
		{
			close(pipe_fd[1]);
			dup2(pipe_fd[0], STDIN_FILENO);
			close(pipe_fd[0]);
			execute_ast(node->right, envp, env_list);
			exit(EXIT_FAILURE);
		}
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		waitpid(pid_left, NULL, 0);
		waitpid(pid_right, NULL, 0);
	}
	else if (node->type == NODE_COMMAND)
	{
		if (is_builtin(node->args[0]) && !needs_child_process(node->args[0]))
		{
			exec_builtin(node->args, env_list, NULL);
			return (0);
		}
		else if ((pid_cmd = fork()) == -1)
		{
			perror("fork");
			return (EXIT_FAILURE);
		}
		if (pid_cmd == 0)
		{
			// if (!process_redirections(node->redirections))
			// {
			// 	perror("redirection");
			// 	exit(EXIT_FAILURE);
			// }
			if (is_builtin(node->args[0]) && needs_child_process(node->args[0]))
			{
				exec_builtin(node->args, env_list, NULL);
				exit(0);
			}
			path = get_path(env_list);
			// if (!path)
			// {
			// 	perror("command not found");
			// 	exit(127);
			// }
			// execve(*path, node->args, envp);
			// perror("execve");
			// exit(EXIT_FAILURE);

			char *cmd_path = NULL;
char *tmp = NULL;
int i = 0;

while (path[i])
{
	tmp = ft_strjoin(path[i], "/");
	cmd_path = ft_strjoin(tmp, node->args[0]);
	free(tmp);
	if (access(cmd_path, X_OK) == 0)
	{
		execve(cmd_path, node->args, envp);
		perror("execve"); // Si execve échoue malgré tout
		exit(EXIT_FAILURE);
	}
	free(cmd_path);
	i++;
}
fprintf(stderr, "%s: command not found\n", node->args[0]);
exit(127);
		}
		waitpid(pid_cmd, NULL, 0);
	}
	return (0);
}


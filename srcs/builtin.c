/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:45:44 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/02 21:42:50 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(void)
{
	// char *cwd;
	char	cwd[PATH_MAX];

	// cwd = getcwd(NULL, 0);
	getcwd(cwd, sizeof(cwd));
	// if (!cwd)
	// {
		// perror("pwd");
		// return (1);
	// }
	printf("%s\n", cwd);
	// free(cwd);
	return (0);
}

int	builtin_env(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (0);
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (args[i] && ft_strncmp(args[i], "-n", 5) == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:45:44 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/14 15:06:59 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	// char *cwd;
	// cwd = getcwd(NULL, 0);
	getcwd(cwd, sizeof(cwd));
	// if (!cwd)
	// {
	// 	perror("pwd");
	// 	return (1);
	// }
	printf("%s\n", cwd);
	// free(cwd);
	return (0);
}

int	builtin_env(t_env *env_list)
{
	t_env	*tmp;

	tmp = env_list;
	// pointeur tmp quon initialise avec la tete de la liste
	while (tmp)
	{
		if (tmp->value)
			// si variable a une valeur,on affiche son nom et sa valur
			printf("%s=%s\n", tmp->name, tmp->value);
		else // sinon on affiche uniquement son nom
			printf("%s\n", tmp->name);
		tmp = tmp->next;
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

void	error_cd(void)
{
	static char	*oldpwd;
	char		*cwd;

	cwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		printf("oldpwd not set");
		free(cwd);
		return ;
	}
	printf("%s\n", oldpwd);
	if (chdir(oldpwd) == -1)
	{
		perror("cd");
		free(cwd);
		return ;
	}
	free(cwd);
	return ;
}

void	ft_cd(char *path)
{
	static char	*oldpwd;
	char		*cwd;

	cwd = getcwd(NULL, 0);
	if (!path || !path[0])
	{
		path = getenv("HOME");
		if (!path)
			return (printf("HOME not set"), free(cwd), free(oldpwd));
	}
	else if (strcmp(path, "-") == 0)
		return (error_cd());
	if (chdir(path) == -1)
	{
		perror("cd");
		free(cwd);
		return ;
	}
	if (oldpwd)
		free(oldpwd);
	oldpwd = cwd;
	free(cwd);
}


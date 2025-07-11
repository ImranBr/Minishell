/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 20:09:47 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/07/11 13:02:09 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	exec_builtin(char **args, t_env *env_list, char *input)
// {
// 	if (ft_strncmp(args[0], "pwd", 4) == 0)
// 		builtin_pwd();
// 	if (ft_strncmp(args[0], "env", 4) == 0)
// 		builtin_env(env_list);
// 	if (ft_strncmp(args[0], "echo", 5) == 0)
// 		builtin_echo(args);
// 	if (ft_strncmp(args[0], "cd", 3) == 0)
// 		ft_cd(args[1]);
// 	if (ft_strncmp(args[0], "export", 7) == 0)
// 		builtin_export(env_list, args);
// 	if (ft_strncmp(args[0], "unset", 6) == 0)
// 		builtin_unset(env_list, args[1]);
// 	if (ft_strncmp(args[0], "exit", 5) == 0)
// 		builtin_exit(env_list, args, input);
// }
void	exec_builtin(char **args, t_env *env_list, char *input)
{
	if (!args || !args[0])
	{
		printf("exec_builtin: args or args[0] is NULL\n");
		return ;
	}
	printf("exec_builtin called with command: '%s'\n", args[0]);
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		builtin_pwd();
	else if (ft_strncmp(args[0], "env", 4) == 0)
		builtin_env(env_list);
	else if (ft_strncmp(args[0], "echo", 5) == 0)
		builtin_echo(args);
	else if (ft_strncmp(args[0], "cd", 3) == 0)
		ft_cd(args[1]);
	else if (ft_strncmp(args[0], "export", 7) == 0)
		builtin_export(env_list, args);
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		builtin_unset(env_list, args[1]);
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		builtin_exit(env_list, args, input);
	else
		printf("exec_builtin: command '%s' not found\n", args[0]);
}

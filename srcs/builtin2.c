/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 20:09:47 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/15 20:08:41 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_builtin(char **args, t_env *env_list)
{
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		builtin_pwd();
	if (ft_strncmp(args[0], "env", 4) == 0)
		builtin_env(env_list);
	if (ft_strncmp(args[0], "echo", 5) == 0)
		builtin_echo(args);
	if (ft_strncmp(args[0], "cd", 3) == 0)
		ft_cd(args[1]);
	if (ft_strncmp(args[0], "export", 7) == 0)
		builtin_export(env_list, args);
}

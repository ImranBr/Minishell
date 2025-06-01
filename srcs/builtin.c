/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:45:44 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/05/31 19:40:57 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_pwd()
{
    char *cwd;
    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("pwd");
        return (1);
    }
    printf("%s\n", cwd);
    free(cwd);
    return (0);
}

int builtin_env(char **env)
{
    int i;
    i = 0;

    while (env && env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
    return (0);
}
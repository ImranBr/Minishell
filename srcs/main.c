/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:06:23 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/05/31 19:43:02 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;

    char *input;
    input = NULL;
    
    while (1)
    {
        input = readline("minishell : ");
        if(!input)
            break;        
        add_history(input);
        if (ft_strncmp(input, "pwd", 4) == 0)
            builtin_pwd();
        if (ft_strncmp(input, "env", 4) == 0)
            builtin_env(env);
        syntax_pipe(input);
        free(input);
    }
    rl_clear_history();
    return 0;
}

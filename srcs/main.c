/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:06:23 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/05/27 00:39:51 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av)
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
        syntax_pipe(input);
    }
    rl_clear_history();
    return 0;
}

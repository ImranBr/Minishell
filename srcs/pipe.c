/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:40:59 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/05/27 01:08:01 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    syntax_pipe(char *input)
{
    int i;

    i = 0;
    while (input[i] == ' ')
        i++;
    if(input[i] == '|')
    {
        ft_putendl_fd("syntax error near unexpected token `|'", STDERR_FILENO);
        return ;
    }
    i = ft_strlen(input) - 1;
    if (input[i] == '|')
        ft_putendl_fd("bash: syntax error: unexpected end of file", STDERR_FILENO);
    i = 0;
    while (input[i])
    {
        if(input[i] == '|' && input[i + 1] == '|')
            ft_putendl_fd("syntax error near unexpected token `|'", STDERR_FILENO);
        i++;
    }
}

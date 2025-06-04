/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:44:19 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/04 11:58:42 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int are_single_quotes_closed(char *input)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (input[i])
    {
        if (input[i] == '\'')
            count ++;
        i++;
    }
    if (count % 2 != 0)
    {
        ft_putendl_fd("syntax error: unclosed single quote", 2);
        return (1);
    }
    else
    return (0);
}

int are_double_quotes_closed(char *input)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (input[i])
    {
        if (input[i] == '\"')
            count ++;
        i++;
    }
    if (count % 2 != 0)
    {
        ft_putendl_fd("syntax error: unclosed double quote", 2);
        return (1);
    }
    else
    return (0);
}
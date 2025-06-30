/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:44:19 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/28 15:59:07 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unclosed_quote_error(char quote)
{
	if (quote == '\'')
		ft_putendl_fd("syntax error: unclosed single quote", STDERR_FILENO);
	else if (quote == '\"')
		ft_putendl_fd("syntax error: unclosed double quote", STDERR_FILENO);
}

int	is_quote_closed(char *input)
{
	int		i;
	char	first_quote;
	int		open;

	i = 0;
	first_quote = 0;
	open = 0;
	while (input[i])
	{
		if (!first_quote && (input[i] == '\'' || input[i] == '\"'))
		{
			first_quote = input[i];
			open = 1;
		}
		else if (input[i] == first_quote)
			open = !open;
		i++;
	}
	if (open)
	{
		unclosed_quote_error(first_quote);
		return (1);
	}
	return (0);
}

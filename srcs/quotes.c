/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:44:19 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/07/06 16:17:13 by ibarbouc         ###   ########.fr       */
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
	char	open_quote;
	int		i;

	i = 0;
	open_quote = 0;
	while (input[i])
	{
		if (!open_quote && (input[i] == '\'' || input[i] == '\"'))
			open_quote = input[i];
		else if (open_quote && input[i] == open_quote)
			open_quote = 0;
		i++;
	}
	if (open_quote)
	{
		unclosed_quote_error(open_quote);
		return (1);
	}
	return (0);
}

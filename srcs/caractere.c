/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caractere.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:46:26 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/05 08:34:42 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char c)
{
	return (c == '|' || c == '&' || c == ';' || c == '>' || c == '<');
}

void	neutralize_special_char_in_single_quote(char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '\''))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		else if (quote && (is_special_char(input[i]) || input[i] == '$'))
			input[i] *= -1;
		i++;
	}
}

void	neutralize_special_char_in_double_quote(char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (input[i])
	{
		if (!quote && input[i] == '"')
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		else if (quote && is_special_char(input[i]))
			input[i] *= -1;
		i++;
	}
}

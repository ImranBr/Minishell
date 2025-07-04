/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:40:59 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/07/03 14:59:53 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char c)
{
	return (c == '|' || c == '&' || c == ';' || c == '>' || c == '<');
}

void	token_error(char c)
{
	write(STDERR_FILENO, "syntax error near unexpected token `", 37);
	write(STDERR_FILENO, &c, 1);
	write(STDERR_FILENO, "'\n", 2);
}

void	syntax_pipe(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ')
		i++;
	if (input[i] == '|')
		return (token_error(input[i]));
	i = ft_strlen(input) - 1;
	while (i >= 0 && (input[i] == ' ' || input[i] == '\t'))
		i--;
	if (i >= 0 && input[i] == '|')
	{
		ft_putendl_fd("bash: syntax error: unexpected end of file",
			STDERR_FILENO);
		return ;
	}
	i = 0;
	while (input[i] && input[i + 1])
	{
		if (is_special_char(input[i]) && is_special_char(input[i + 1]))
			return (token_error(input[i + 1]));
		i++;
	}
}

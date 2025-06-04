/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caractere.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:46:26 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/04 14:17:06 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char c)
{
	return (c == '|' || c == '&' || c == ';' || c == '>' || c == '<');
}

int	neutralize_special_char_in_quote(char *input)
{
	int	i;
	int	in_single;

	i = 0;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 16:21:52 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/20 20:34:51 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while ((s1[i] == s2[i] && s1[i] != '\0') && (s2[i] != '\0'))
		i++;
	return (s1[i] - s2[i]);
}

t_env	*free_list(t_env *a)
{
	t_env	*next_node;

	if (!a)
		return (NULL);
	while (a)
	{
		next_node = a->next;
		free(a->name);
		free(a->value);
		free(a);
		a = next_node;
	}
	return (NULL);
}

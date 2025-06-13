/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:10:02 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/13 16:31:11 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char *name, t_env *env_list)
{
	t_env	*current;

	if (!name || !env_list)
		return (NULL);
	current = env_list;
	while (current)
	{
		if (current->name && ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

t_env	create_env_list(char **envp)
{
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	while (envp[i])
	{
		i++;
		while (envp[j] != '=')
			j++;
	}
}
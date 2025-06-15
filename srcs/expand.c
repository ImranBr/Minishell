/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:10:02 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/15 20:04:11 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*get_env_value(char *name, t_env *env_list)
// {
// 	t_env	*current;

// 	if (!name || !env_list)
// 		return (NULL);
// 	current = env_list;
// 	while (current)
// 	{
// 		if (current->name && ft_strcmp(current->name, name) == 0)
// 			return (current->value);
// 		current = current->next;
// 	}
// 	return (NULL);
// }

void	add_env_node(t_env **env_list, char *name, char *value)
{
	t_env	*new_node;
	t_env	*tmp;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->name = name;
	new_node->value = value;
	new_node->next = NULL;
	if (*env_list == NULL)
		*env_list = new_node;
	else
	{
		tmp = *env_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

t_env	*create_env_list(char **envp)
{
	int		i;
	int		j;
	char	*name;
	char	*value;
	t_env	*env_list;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j])
		{
			if (envp[i][j] == '=')
			{
				name = ft_substr(envp[i], 0, j);
				value = ft_substr(envp[i], j + 1, ft_strlen(envp[i]));
				add_env_node(&env_list, name, value);
				break ;
			}
			j++;
		}
		i++;
	}
	return (env_list);
}

// char	*expand_variables(char *input, t_env *env_list, int exit_status)
// {
// 	int		i;
// 	int		in_single_quote;
// 	int		in_double_quote;
// 	char	*result;

// 	i = 0;
// 	in_single_quote = 0;
// 	in_double_quote = 0;
// 	while (input[i])
// 	{
// 		if (input[i] == '\'' && !in_double_quote)
// 			in_single_quote = !in_single_quote;
// 		else if (input[i] == '\"' && !in_single_quote)
// 			in_double_quote = !in_double_quote;
// 	}
// 	return (result);
// }

// int	is_valid_var_char(char c)
// {
// 	return (ft_isalnum(c) || c == '_');
// }

// int	extract_var_name(char *input, int start_index, char *var_name_buffer)
// {
// 	int i;

// 	i = 0;
// 	if (input[start_index] == '?')
// 	{
// 		var_name_buffer[0] = '?';
// 		var_name_buffer[1] = '\0';
// 		return (1);
// 	}
// 	while (is_valid_var_char(input[start_index + i]))
// 	{
// 		var_name_buffer[i] = input[start_index + i];
// 		i++;
// 	}
// 	var_name_buffer[i] = '\0';
// 	return (i);
// }

// char	*expand_dollar(char *input, t_env *env_list, int exit_status)
// {
	
// }
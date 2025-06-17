/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:10:02 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/17 21:31:54 by ibarbouc         ###   ########.fr       */
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

void	add_env_node(t_env **env_list, char *name, char *value)
{
	t_env	*new_node;
	t_env	*tmp;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->name = ft_strdup(name);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
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
	free(name);
	free(value);
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

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	extract_var_name(char *input, int start_index, char *var_name_buffer)
{
	int i;

	i = 0;
	if (input[start_index] == '?')
	{
		var_name_buffer[0] = '?';
		var_name_buffer[1] = '\0';
		return (1);
	}
	while (is_valid_var_char(input[start_index + i]))
	{
		var_name_buffer[i] = input[start_index + i];
		i++;
	}
	var_name_buffer[i] = '\0';
	return (i);
}

char	*expand_dollar(char *input, int *i, t_env *env_list, int exit_status)
{
	
    (*i)++;
    if (input[*i] == '?')
    {
        (*i)++;
        return ft_itoa(exit_status);
    }
    char var_name[256];
    int len = extract_var_name(input, *i, var_name);
    *i += len;
    char *value = get_env_value(var_name, env_list);
    if (!value)
        return ft_strdup("");
    return ft_strdup(value);
}

char *expand_variables(char *input, t_env *env_list, int exit_status)
{
	int		i = 0;
	int		in_single_quote = 0;
	int		in_double_quote = 0;
	char	*result = ft_strdup("");

	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			result = ft_strjoin(result, "\'");
		}
		else if (input[i] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			result = ft_strjoin(result, "\"");
		}
		else if (input[i] == '$' && !in_single_quote)
		{
			char *expanded = expand_dollar(input, &i, env_list, exit_status);
			result = ft_strjoin(result, expanded);
			free(expanded);
			continue;
		}
		else
		{
			char temp[2] = {input[i], 0};
			result = ft_strjoin(result, temp);
		}
		i++;
	}
	return result;
}


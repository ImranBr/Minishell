/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:10:02 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/22 00:57:41 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	extract_var_name(char *input, int start_index, char *var_name_buffer)
{
	int	i;

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
	char	var_name[256];
	int		len;
	char	*value;

	(*i)++;
	if (input[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(exit_status));
	}
	len = extract_var_name(input, *i, var_name);
	*i += len;
	value = get_env_value(var_name, env_list);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*expand_variables(char *input, t_env *env_list, int exit_status)
{
	t_expand	exp;

	exp.i = 0;
	exp.in_single_quote = 0;
	exp.in_double_quote = 0;
	exp.result = ft_strdup("");

	while (input[exp.i])
	{
		if (input[exp.i] == '\'' && !exp.in_double_quote)
			exp.in_single_quote = !exp.in_single_quote;
		else if (input[exp.i] == '"' && !exp.in_single_quote)
			exp.in_double_quote = !exp.in_double_quote;
		else if (input[exp.i] == '$' && !exp.in_single_quote)
		{
			exp.expanded = expand_dollar(input, &exp.i, env_list, exit_status);
			exp.tmp = exp.result;
			exp.result = ft_strjoin(exp.result, exp.expanded);
			free(exp.tmp);
			free(exp.expanded);
			continue ;
		}
		else
		{
			exp.temp[0] = input[exp.i];
			exp.temp[1] = '\0';
			exp.tmp = exp.result;
			exp.result = ft_strjoin(exp.result, exp.temp);
			free(exp.tmp);
		}
		exp.i++;
	}
	return (exp.result);
}

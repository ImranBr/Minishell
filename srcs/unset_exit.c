/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:45:44 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/22 19:05:46 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_num(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_valid_nbr(char *str)
{
	int			i;
	int			signe;
	long int	result;

	i = 0;
	signe = 1;
	result = 0;
	if (str[0] == '-' || str[0] == '+')
	{
		if (str[0] == '-')
			signe = -1;
		i++;
	}
	if (str[i] == '\0')
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		result = result * 10 + (str[i] - 48);
		if ((result * signe) < INT_MIN || (result * signe) > INT_MAX)
			return (2);
		i++;
	}
	return (0);
}

void	builtin_exit(char **args)
{
	long	exit_code;

	printf("exit\n");
	if (!args[1])
		exit(0);
	if (!ft_atoi(args[1]))
	{
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required", STDERR_FILENO);
		exit(2);
	}
	if (args[2])
	{
		ft_putstr_fd("exit: too many arguments", STDERR_FILENO);
		exit(2);
	}
	exit_code = ft_valid_nbr(args[1]);
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code += 256;
	exit(exit_code);
}

t_env	*builtin_unset(t_env *env, char *cmd)
{
	t_env	*previous;
	t_env	*current;

	previous = NULL;
	current = env;
	while (current)
	{
		if (ft_strcmp(current->name, cmd) == 0)
		{
			if (previous)
				previous->next = current->next;
			else
				env = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return (0);
		}
		previous = current;
		current = current->next;
	}
	return (env);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:06:23 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/07 17:30:45 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*input;
	char	**args;

	(void)ac;
	(void)av;
	input = NULL;
	args = NULL;
	while (1)
	{
		input = readline("minishell : ");
		if (!input)
			break ;
		add_history(input);
		args = ft_split(input, ' ');
		if (!args || !args[0])
		{
			free(input);
			free_split(args);
			continue ;
		}
		if (are_double_quotes_closed(input) != 0
			|| are_single_quotes_closed(input) != 0)
		{
			free(input);
			free_split(args);
			return (1);
		}
		neutralize_special_char_in_double_quote(input);
		neutralize_special_char_in_single_quote(input);
		if (ft_strncmp(args[0], "pwd", 4) == 0)
			builtin_pwd();
		if (ft_strncmp(args[0], "env", 4) == 0)
			builtin_env(env);
		if (ft_strncmp(args[0], "echo", 5) == 0)
			builtin_echo(args);
		syntax_special_char(input);
		free(input);
		free_split(args);
	}
	rl_clear_history();
	return (0);
}

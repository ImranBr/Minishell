/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:06:23 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/28 12:44:10 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	*expanded;
	char	**args;
	t_env	*env_list;

	(void)ac;
	(void)av;
	input = NULL;
	args = NULL;
	env_list = create_env_list(envp);
	while (1)
	{
		input = readline("minishell : ");
		if (!input)
			break ;
		add_history(input);
		if (is_quote_closed(input)!= 0)
		{
			free(input);
			continue ;
		}
		expanded = expand_variables(input, env_list, g_exit_status);
		free(input);
		input = expanded;
		neutralize_special_char_in_double_quote(input);
		neutralize_special_char_in_single_quote(input);
		printf("minishell : %s\n", input);
		args = ft_split(input, ' ');
		if (!args || !args[0])
		{
			free(input);
			free_split(args);
			continue ;
		}
		exec_builtin(args, env_list, input);
		syntax_special_char(input);
		free(input);
		free_split(args);
	}
	rl_clear_history();
	free_list(env_list);
	return (0);
}

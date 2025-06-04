/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:06:23 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/04 12:13:32 by ibarbouc         ###   ########.fr       */
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
		if (are_double_quotes_closed(input) != 0 || are_single_quotes_closed(input) != 0)
			return (1);
		if (ft_strncmp(args[0], "pwd", 4) == 0)
			builtin_pwd();
		if (ft_strncmp(args[0], "env", 4) == 0)
			builtin_env(env);
		if (ft_strncmp(args[0], "echo", 5) == 0)
			builtin_echo(args);
		syntax_pipe(input);
		free(input);
		free_split(args);
	}
	rl_clear_history();
	return (0);
}

// int main()
// {
//     char *s1 = "ceci 'est \"un test";
//     char *s2 = "ceci est un test'";
//     printf("s1 single quotes = %d\ns1 double quotes = %d\n", are_single_quotes_closed(s1), are_double_quotes_closed(s1));
//     printf("s2 single quotes = %d\ns2 double quotes = %d\n", are_single_quotes_closed(s2), are_double_quotes_closed(s2));
// }
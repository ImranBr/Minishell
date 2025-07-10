/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:06:23 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/07/10 15:53:04 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

// int	main(int ac, char **av, char **envp)
// {
// 	char	*input;
// 	char	*expanded;
// 	char	**args;
// 	t_env	*env_list;
// 	int		exit_status;

// 	exit_status = 0;
// 	(void)ac;
// 	(void)av;
// 	input = NULL;
// 	args = NULL;
// 	env_list = create_env_list(envp);
// 	while (1)
// 	{
// 		input = readline("minishell : ");
// 		if (!input)
// 			break ;
// 		add_history(input);
// 		if (is_quote_closed(input) != 0)
// 		{
// 			free(input);
// 			continue ;
// 		}
// 		expanded = expand_variables(input, env_list, exit_status);
// 		free(input);
// 		input = expanded;
// 		// printf("minishell : %s\n", input);
// 		args = ft_split(input, ' ');
// 		if (!args || !args[0])
// 		{
// 			free(input);
// 			free_split(args);
// 			continue ;
// 		}
// 		exec_builtin(args, env_list, input);
// 		syntax_pipe(input);
// 		free(input);
// 		free_split(args);
// 	}
// 	rl_clear_history();
// 	free_list(env_list);
// 	return (0);
// }

int	main(int ac, char **av, char **envp)
{
	char		*input;
	char		*expanded;
	char		**args;
	t_env		*env_list;
	int			exit_status;
	t_token		*tokens;
	t_token		*tmp;
	t_ast_node	*ast;
	bool		branches[100] = {0};

	exit_status = 0;
	(void)ac;
	(void)av;


	env_list = create_env_list(envp);
	while (1)
	{
		input = readline("minishell : ");
		if (!input)
			break ;
		add_history(input);
		if (is_quote_closed(input) != 0)
		{
			free(input);
			continue ;
		}
		expanded = expand_variables(input, env_list, exit_status);
		free(input);
		input = expanded;

		// Tokenisation
		tokens = tokenize(input);
		printf("Tokens:\n");
		tmp = tokens;
		while (tmp && tmp->type != EOF_TOKEN)
		{
			printf("  type=%d, value='%s'\n", tmp->type, tmp->value);
			tmp = tmp->next;
		}
		// Parsing
		ast = parse_pipeline(&tokens);
		if (!ast)
		{
			fprintf(stderr, "Erreur de parsing\n");
			free_tokens(tokens);
			free(input);
			continue ;
		}

		printf("\nAST:\n");
		print_ast(ast, 0, true, branches);
		// --- Nouvelle partie : gestion builtin ---
		args = ft_split(input, ' ');
		if (args && args[0])
		{
			// exec_builtin(args, env_list, input);
			execute_ast(ast, envp, env_list);
			// Si builtin exécuté, on ne fait pas d'exécution AST
			free_split(args);
			free_ast(ast);
			free_tokens(tokens);
			free(input);
			continue ;	
		}
		free_split(args);
		// Ici : exécution de l'AST (non montrée
		free_ast(ast);
		free_tokens(tokens);
		free(input);
	}
	rl_clear_history();
	free_list(env_list);
	return (0);
}

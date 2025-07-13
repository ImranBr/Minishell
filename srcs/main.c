/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:44:21 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/13 17:31:20 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

pid_t	g_signal_pid = 0;

void	check_signal(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// int	main(int ac, char **av, char **envp)
// {
// 	char *input;
// 	char *expanded;
// 	char **args;
// 	t_env *env_list;
// 	int exit_status;
// 	t_token *tokens;
// 	t_token *tokens_head;// <-- Nouvelle variable pour garder la tête de la liste
// 	t_token *tmp;
// 	t_ast_node *ast;
// 	// bool		branches[100] = {0};

// 	exit_status = 0;
// 	(void)ac;
// 	(void)av;
// 	env_list = create_env_list(envp);
// 	while (1)
// 	{
// 		signal(SIGINT, check_signal);
// 		signal(SIGQUIT, SIG_IGN);
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
// 		// Tokenisation
// 		tokens = tokenize(input, 0);
// 		tokens_head = tokens; // <-- Garde le début de la liste
// 		tmp = tokens;
// 		while (tmp && tmp->type != EOF_TOKEN)
// 		{
// 			printf("  type=%d, value='%s'\n", tmp->type, tmp->value);
// 			tmp = tmp->next;
// 		}
// 		// Parsing
// 		ast = parse_pipeline(&tokens);
// 		if (!ast)
// 		{
// 			fprintf(stderr, "Erreur de parsing\n");
// 			free_tokens(tokens_head); // <-- Toujours libérer depuis la tête
// 			free(input);
// 			continue ;
// 		}
// 		// --- Nouvelle partie : gestion builtin ---
// 		args = ft_split(input, ' ');
// 		if (args && args[0])
// 		{
// 			execute_ast(ast, envp, env_list);
// 			// Si builtin exécuté, on ne fait pas d'exécution AST
// 			free_split(args);
// 			free_ast(ast);
// 			free_tokens(tokens_head); // <-- Toujours libérer depuis la tête
// 			free(input);
// 			continue ;
// 		}
// 		free_split(args);
// 		// Ici : exécution de l'AST (non montrée)
// 		free_ast(ast);
// 		free_tokens(tokens_head); // <-- Toujours libérer depuis la tête
// 		free(input);
// 	}
// 	rl_clear_history();
// 	env_list = free_list(env_list);
// 	return (0);
// }

// int	main(int ac, char **av, char **envp)
// {
// 	char		*input;
// 	char		*expanded;
// 	char		**args;
// 	t_env		*env_list;
// 	int			exit_status;
// 	t_token		*tokens;
// 	t_token		*tokens_head;
// 	t_token		*tmp;
// 	t_ast_node	*ast;

// 	input = NULL;
// 	expanded = NULL;
// 	args = NULL;
// 	exit_status = 0;
// 	tokens = NULL;
// 	tokens_head = NULL;
// 	tmp = NULL;
// 	ast = NULL;
// 	(void)ac;
// 	(void)av;
// 	env_list = create_env_list(envp);
// 	while (1)
// 	{
// 		// Réinitialisation à NULL à chaque boucle pour éviter des free accidentels
// 		input = expanded = NULL;
// 		args = NULL;
// 		tokens = tokens_head = NULL;
// 		ast = NULL;
// 		signal(SIGINT, check_signal);
// 		signal(SIGQUIT, SIG_IGN);
// 		input = readline("minishell : ");
// 		if (!input)
// 			break ;
// 		add_history(input);
// 		// GESTION QUOTES
// 		if (is_quote_closed(input) != 0)
// 		{
// 			free(input);
// 			continue ;
// 		}
// 		// EXPANSION
// 		expanded = expand_variables(input, env_list, exit_status);
// 		free(input);
// 		input = expanded;
// 		// TOKENISATION
// 		tokens = tokenize(input, 0);
// 		tokens_head = tokens;
// 		tmp = tokens;
// 		while (tmp && tmp->type != EOF_TOKEN)
// 		{
// 			printf("  type=%d, value='%s'\n", tmp->type, tmp->value);
// 			tmp = tmp->next;
// 		}
// 		// PARSING
// 		ast = parse_pipeline(&tokens);
// 		if (!ast)
// 		{
// 			fprintf(stderr, "Erreur de parsing\n");
// 			free_tokens(tokens_head);
// 			free(input);
// 			continue ;
// 		}
// 		// BUILTIN HANDLING
// 		args = ft_split(input, ' ');
// 		if (args && args[0])
// 		{
// 			execute_ast(ast, envp, env_list);
// 			free_split(args);
// 			free_ast(ast);
// 			free_tokens(tokens_head);
// 			free(input);
// 			continue ;
// 		}
// 		free_split(args);
// 		free_ast(ast);
// 		free_tokens(tokens_head);
// 		free(input);
// 	}
// 	rl_clear_history();
// 	env_list = free_list(env_list);
// 	return (0);
// }

int	main(int ac, char **av, char **envp)
{
	char		*input;
	char		*expanded;
	t_env		*env_list;
	int			exit_status;
	t_token		*tokens;
	t_token		*tokens_head;
	t_ast_node	*ast;

	input = NULL;
	expanded = NULL;
	env_list = NULL;
	exit_status = 0;
	tokens = NULL;
	tokens_head = NULL;
	ast = NULL;
	(void)ac;
	(void)av;
	env_list = create_env_list(envp);
	while (1)
	{
		// Réinitialisation à NULL pour éviter un free accidentel
		input = expanded = NULL;
		tokens = tokens_head = NULL;
		ast = NULL;
		signal(SIGINT, check_signal);
		signal(SIGQUIT, SIG_IGN);
		input = readline("minishell : ");
		if (!input)
			break ;
		add_history(input);
		// Gestion quotes
		if (is_quote_closed(input) != 0)
		{
			free(input);
			continue ;
		}
		// Expansion des variables
		expanded = expand_variables(input, env_list, exit_status);
		free(input);
		input = expanded;
		// Tokenisation
		tokens = tokenize(input, 0);
		tokens_head = tokens;
		// Parsing
		ast = parse_pipeline(&tokens);		
		if (!ast)
		{
			free_ast(ast);
			free_tokens(tokens_head);
			free(input);
			continue ;
		}
		// Gestion builtin (exemple)

		execute_ast(ast, envp, env_list, tokens_head);

		// Exécution AST générale (non montrée)
		free_ast(ast);
		free_tokens(tokens_head);
		free(input);
	}
	rl_clear_history();
	env_list = free_list(env_list);
	return (0);
}

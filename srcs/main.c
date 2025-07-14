/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:44:21 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/14 17:27:09 by joudafke         ###   ########.fr       */
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

char *readline_stderr(const char *prompt)
{
	int		saved_stdout;
	int 	devnull;
	char	*line;

	saved_stdout = dup(STDOUT_FILENO);
	devnull = open("/dev/null", O_WRONLY);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	line = readline(prompt);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	close(devnull);
	return line;
}

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
		input = readline_stderr("minishell : ");
		if (!input)
		{
			free(input);
			break ;
		}
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

		execute_ast(ast, envp, env_list, tokens_head, input);

		// Exécution AST générale (non montrée)
		free_ast(ast);
		free_tokens(tokens_head);
		free(input);
	}
	free(input);
	rl_clear_history();
	env_list = free_list(env_list);
	return (0);
}

// int	main(int ac, char **av, char **envp)
// {
// 	char		*input = NULL;
// 	char		*expanded = NULL;
// 	char		*last_expanded = NULL;
// 	t_env		*env_list = NULL;
// 	int			exit_status = 0;
// 	t_token		*tokens = NULL;
// 	t_token		*tokens_head = NULL;
// 	t_ast_node	*ast = NULL;

// 	(void)ac;
// 	(void)av;
// 	env_list = create_env_list(envp);
// 	while (1)
// 	{
// 		// Libère le buffer de la dernière expansion (jamais free deux fois)
// 		if (last_expanded) {
// 			free(last_expanded);
// 			last_expanded = NULL;
// 		}
// 		tokens = tokens_head = NULL;
// 		ast = NULL;
// 		signal(SIGINT, check_signal);
// 		signal(SIGQUIT, SIG_IGN);
// 		input = readline("minishell : ");
// 		if (!input)
// 			break;
// 		add_history(input);
// 		// Gestion quotes
// 		if (is_quote_closed(input) != 0)
// 		{
// 			free(input);
// 			continue ;
// 		}
// 		// Expansion des variables
// 		expanded = expand_variables(input, env_list, exit_status);
// 		free(input);
// 		input = expanded;
// 		last_expanded = input; // Toujours garder le dernier buffer pour free
// 		// Tokenisation
// 		tokens = tokenize(input, 0);
// 		tokens_head = tokens;
// 		// Parsing
// 		ast = parse_pipeline(&tokens);		
// 		if (!ast)
// 		{
// 			free_ast(ast);
// 			free_tokens(tokens_head);
// 			// Ici, pas besoin de free(input), c'est last_expanded qui gère
// 			continue ;
// 		}
// 		// Gestion builtin (exemple)
// 		execute_ast(ast, envp, env_list, tokens_head);
// 		// Exécution AST générale (non montrée)
// 		free_ast(ast);
// 		free_tokens(tokens_head);
// 		// Ici, pas besoin de free(input), c'est last_expanded qui gère
// 	}
// 	// Libère la dernière expansion si besoin (cas où on quitte par break)
// 	if (last_expanded)
// 		free(last_expanded);
// 	rl_clear_history();
// 	env_list = free_list(env_list);
// 	return (0);
// }

// #define BUF_SIZE 1024

// int	main(int ac, char **av, char **envp)
// {
// 	char		*input = NULL;
// 	char		*expanded = NULL;
// 	char		*last_expanded = NULL;
// 	char		buf[BUF_SIZE];
// 	t_env		*env_list = NULL;
// 	int			exit_status = 0;
// 	t_token		*tokens = NULL;
// 	t_token		*tokens_head = NULL;
// 	t_ast_node	*ast = NULL;

// 	(void)ac;
// 	(void)av;
// 	env_list = create_env_list(envp);
// 	while (1)
// 	{
// 		if (last_expanded) {
// 			free(last_expanded);
// 			last_expanded = NULL;
// 		}
// 		tokens = tokens_head = NULL;
// 		ast = NULL;
// 		printf("minishell : ");
// 		if (!fgets(buf, BUF_SIZE, stdin))
// 			break;
// 		// Retire le \n éventuel
// 		buf[strcspn(buf, "\n")] = 0;
// 		if (strlen(buf) == 0)
// 			continue;
// 		input = strdup(buf);
// 		// Gestion quotes
// 		if (is_quote_closed(input) != 0)
// 		{
// 			free(input);
// 			continue ;
// 		}
// 		// Expansion des variables
// 		expanded = expand_variables(input, env_list, exit_status);
// 		free(input);
// 		input = expanded;


// 		last_expanded = input;
// 		// Tokenisation
// 		tokens = tokenize(input, 0);
// 		tokens_head = tokens;
// 		// Parsing
// 		ast = parse_pipeline(&tokens);		
// 		if (!ast)
// 		{
// 			free_ast(ast);
// 			free_tokens(tokens_head);
// 			continue ;
// 		}
// 		execute_ast(ast, envp, env_list, tokens_head);
// 		free_ast(ast);
// 		free_tokens(tokens_head);
// 	}
// 	// Libère la dernière expansion si besoin (cas où on quitte par break)
// 	if (last_expanded)
// 	{
// 		printf("hhhghjhjkshdjhskhdkshkdhskhfkshfkshfkkhfsjhkfhk\n");
// 		free(last_expanded);
// 	}
// 		env_list = free_list(env_list);
// 	return (0);
// }


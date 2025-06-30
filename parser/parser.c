/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:33:48 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/01 00:06:43 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast_node	*create_redir_node(t_ast_node *cmd, t_token *token_list)
{
	t_ast_node		*redir;
	t_token_type	redir_type;

	redir = NULL;
	redir_type = (token_list)->type;
	token_list = (token_list)->next;
	if (!token_list || (token_list)->type != WORD)
	{
		write(STDERR_FILENO, "syntax error\n", 13);
		free_ast(cmd);
		return (NULL);
	}
	redir = create_ast_node(NODE_REDIRECT);
	redir->redir_type = redir_type;
	redir->filename = ft_strdup((token_list)->value);
	return (redir);
}

t_ast_node	*parse_command(t_token **token_list)
{
	t_ast_node		*cmd;
	t_ast_node		*redir;

	cmd = NULL;
	if (!token_list || !*token_list)
		return (NULL);
	while (*token_list && (*token_list)->type != PIPE)
	{
		if ((*token_list)->type == WORD)
		{
			if (!cmd)
				cmd = create_ast_node(NODE_COMMAND);
			add_args_to_cmd(cmd, ft_strdup((*token_list)->value));
		}
		else if ((*token_list)->type == REDIRECT_IN
			|| (*token_list)->type == REDIRECT_OUT
			|| (*token_list)->type == APPEND || (*token_list)->type == HEREDOC)
		{
			if (!cmd)
				cmd = create_ast_node(NODE_COMMAND);
			redir = create_redir_node(cmd, *token_list);
			add_redir_to_cmd(cmd, redir);
			*token_list = (*token_list)->next;
		}
		*token_list = (*token_list)->next;
	}
	return (cmd);
}

t_ast_node	*parse_pipeline(t_token **token_list)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_ast_node	*node_pipe;

	if (*token_list && (*token_list)->type == PIPE)
	{
		write(STDERR_FILENO, "syntax error near unexpected token `|'\n", 39);
		return (NULL);
	}
	left = parse_command(token_list);
	if (!left)
		return (NULL);
	while (*token_list && (*token_list)->type == PIPE)
	{
		*token_list = (*token_list)->next;
		right = parse_command(token_list);
		if (!right)
		{
			write(STDERR_FILENO, "syntax error\n", 13);
			free_ast(left);
			return (NULL);
		}
		node_pipe = create_ast_node(NODE_PIPE);
		node_pipe->left = left;
		node_pipe->right = right;
		left = node_pipe;
	}
	return (left);
}

// Implémentation de ft_strdup (si pas déjà incluse)
char *ft_strdup(const char *s)
{
	int len = strlen(s) + 1;
	char *copy = malloc(len);
	if (copy)
		memcpy(copy, s, len);
	return copy;
}

// Implémentation de ft_strlen (si pas déjà incluse)
int ft_strlen(const char *s)
{
	int len = 0;
	while (s[len])
		len++;
	return len;
}

// Implémentation de ft_substr (utilisée par le tokenizer)
char *ft_substr(const char *s, unsigned int start, int len)
{
	char *sub;
	int i = 0;

	if (start >= ft_strlen(s))
		return ft_strdup("");

	sub = malloc(len + 1);
	if (!sub)
		return NULL;

	while (i < len && s[start + i])
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return sub;
}

// // Fonction d'affichage récursif de l'AST
// void print_ast(t_ast_node *node, int level)
// {
// 	if (!node)
// 		return;

// 	for (int i = 0; i < level; i++)
// 		printf("  ");

// 	if (node->type == NODE_PIPE)
// 	{
// 		printf("PIPE\n");
// 		print_ast(node->left, level + 1);
// 		print_ast(node->right, level + 1);
// 	}
// 	else if (node->type == NODE_COMMAND)
// 	{
// 		printf("COMMAND\n");
// 		for (int i = 0; i < node->args_count; i++)
// 		{
// 			for (int j = 0; j <= level; j++)
// 				printf("  ");
// 			printf("arg[%d]: %s\n", i, node->args[i]);
// 		}
// 		t_ast_node *redir = node->right;
// 		while (redir)
// 		{
// 			for (int j = 0; j <= level; j++)
// 				printf("  ");
// 			printf("REDIRECT (%d): %s\n", redir->type, redir->filename);
// 			redir = redir->right;
// 		}
// 	}
// }

void print_indent(int level, bool is_last, bool *branches)
{
	for (int i = 0; i < level; i++)
	{
		if (branches[i])
			printf("│   ");
		else
			printf("    ");
	}

	if (is_last)
		printf("└── ");
	else
		printf("├── ");
}

void print_ast(t_ast_node *node, int level, bool is_last, bool *branches)
{
	if (!node)
		return;

	print_indent(level, is_last, branches);

	if (node->type == NODE_PIPE)
	{
		printf("PIPE\n");
		branches[level] = !is_last;  // Marque qu'on continue à dessiner la colonne si ce n'est pas le dernier enfant
		print_ast(node->left, level + 1, false, branches);
		print_ast(node->right, level + 1, true, branches);
	}
	else if (node->type == NODE_COMMAND)
	{
		printf("COMMAND\n");
		for (int i = 0; i < node->args_count; i++)
		{
			print_indent(level + 1, i == node->args_count - 1 && node->right == NULL, branches);
			printf("arg[%d]: %s\n", i, node->args[i]);
		}

		t_ast_node *redir = node->right;
		int redir_index = 0;
		while (redir)
		{
			bool last_redir = (redir->right == NULL);
			print_indent(level + 1, last_redir, branches);
			printf("REDIRECT (%d): %s\n", redir->type, redir->filename);
			redir = redir->right;
			redir_index++;
		}
	}
}

// int main(void)
// {
// 	char input[] = "ls -l | grep txt > out.txt";
// 	t_token *tokens = tokenize(input);

// 	printf("Tokens:\n");
// 	for (t_token *t = tokens; t && t->type != EOF_TOKEN; t = t->next)
// 		printf("  type=%d, value='%s'\n", t->type, t->value);

// 	t_ast_node *ast = parse_pipeline(&tokens);
// 	if (!ast)
// 	{
// 		fprintf(stderr, "Erreur de parsing\n");
// 		free_tokens(tokens);
// 		return (1);
// 	}

// 	printf("\nAST:\n");
// 	print_ast(ast, 0);

// 	free_ast(ast);
// 	free_tokens(tokens);
// 	return 0;
// }

int main(void)
{
	char input[] = ">> output.txt | ls -l | grep rfjd kfpvlf txt > out.txt | cat out.txt | echo \"salut\" >> out.txt";
	t_token *tokens = tokenize(input);

	printf("Tokens:\n");
	for (t_token *t = tokens; t && t->type != EOF_TOKEN; t = t->next)
		printf("  type=%d, value='%s'\n", t->type, t->value);

	t_ast_node *ast = parse_pipeline(&tokens);
	if (!ast)
	{
		fprintf(stderr, "Erreur de parsing\n");
		free_tokens(tokens);
		return 1;
	}

	printf("\nAST:\n");

	// Initialisation du tableau des branches
	bool branches[100] = {0};
	print_ast(ast, 0, true, branches);

	free_ast(ast);
	free_tokens(tokens);
	return 0;
}

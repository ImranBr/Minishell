/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:07:31 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/10 15:36:09 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>

# include <stdbool.h>

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	APPEND,
	REDIRECT_OUT,
	HEREDOC,
	EOF_TOKEN
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIRECT
}	t_node_type;

typedef struct s_ast_node
{
	t_node_type			type;
	char				**args;
	int					args_count;
	char				*filename;
	t_token_type		redir_type;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	struct s_ast_node	*redirections;
}						t_ast_node;

/* TOKENIZATION */
int			is_space(char c);
int			is_operator(char c);
t_token		*create_token(t_token_type token_type, char *value);
void		add_token(t_token **token_list, t_token *new_token);
void		free_tokens(t_token *token_list);

int			tokenize_operators(char *input, t_token **token_list, int i);
int			ending_quotes(char *input, int start);
int			tokenize_words(char *input, t_token **token_list, size_t start_index);
t_token		*tokenize(char *input);

/* PARSING */
t_ast_node	*create_ast_node(t_node_type node_type);
int			add_args_to_cmd(t_ast_node *cmd, char *arg);
void		add_redir_to_cmd(t_ast_node *cmd, t_ast_node *redir);
void		free_ast(t_ast_node *node);
t_ast_node	*parse_command(t_token **token_list);
t_ast_node	*parse_pipeline(t_token **token_list);
void		*process_redirections(t_ast_node *redir_list);

void		print_indent(int level, bool is_last, bool *branches);
void		print_ast(t_ast_node *node, int level, bool is_last, bool *branches);
char		*ft_strdup(const char *s);

// int			ft_strlen(const char *s);
// char		*ft_substr(const char *s, unsigned int start, int len);

#endif
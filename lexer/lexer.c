/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:00:47 by joudafke          #+#    #+#             */
/*   Updated: 2025/06/15 19:58:06 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '(' || c == ')' || c == '&')
		return (1);
	return (0);
}

int	ending_quotes(char *input, int start)
{
	char	quote;
	int		i;

	quote = input[start];
	i = start + 1;
	while (input[i] && input[i] != quote)
		i++;
	return (i);
}

t_token	*create_token(t_token_type token_type, char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = token_type;
	if (!value)
		new_token->value = NULL;
	else
		new_token->value = ft_strdup(value);
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **token_list, t_token *new_token)
{
	t_token	*tmp;

	if (!*token_list)
	{
		*token_list = new_token;
		return ;
	}
	tmp = *token_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
}

t_token	*tokenize(char *input)
{
	t_token	*token_list;
	int		i;
	int		len_input;
	char	c;
	int		start_word;
	int		end_word;
	char	*word_value;

	token_list = NULL;
	i = 0;
	len_input = ft_strlen(input);
	while (i < len_input)
	{
		c = input[i];
		if (is_space(c))
			add_token(&token_list, create_token(SPACE, " "));
		else if (c == '|')
			add_token(&token_list, create_token(PIPE, "|"));
		else if (c == '<')
		{
			if (input[i + 1] == '<')
			{
				add_token(&token_list, create_token(APPEND, "<<"));
				i++;
			}
			else
				add_token(&token_list, create_token(REDIRECT_IN, "<"));
		}
		else if (c == '>')
		{
			if (input[i + 1] == '>')
			{	
				add_token(&token_list, create_token(HEREDOC, ">>"));
				i++;
			}
			else
				add_token(&token_list, create_token(REDIRECT_OUT, ">"));
		}
		else
		{
			if (c == '"' || c == '\'')
			{
				end_word = ending_quotes(input, i);
				word_value = ft_substr(input, i + 1, end_word - i - 1);
				add_token(&token_list, create_token(WORD, word_value));
				i = end_word;
			}
			else
			{
				start_word = i;
				while (i < len_input && !is_space(input[i])
					&& !is_operator(input[i]))
					i++;
				word_value = ft_substr(input, start_word, i - start_word);
				add_token(&token_list, create_token(WORD, word_value));
				i--;
			}
			free(word_value);
		}
		i++;
	}
	add_token(&token_list, create_token(EOF_TOKEN, NULL));
	return (token_list);
}

void	free_tokens(t_token *token_list)
{
	t_token	*tmp;
	while (token_list)
	{
		tmp = token_list;
		token_list = token_list->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

// int	main(void)
// {
// 	char	*line = "echo \"Hello world\" >> file.txt";
// 	t_token	*tokens = tokenize(line);
// 	t_token	*current = tokens;

// 	while (current)
// 	{
// 		printf("Type: %d, Value: %s\n", current->type, current->value);
// 		current = current->next;
// 	}
// 	free_tokens(tokens);
// 	return (0);
// }
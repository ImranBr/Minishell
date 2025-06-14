/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:00:47 by joudafke          #+#    #+#             */
/*   Updated: 2025/06/13 01:13:28 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	isspace(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
			return (1);
		i++;
	}
	return (0);
}

int	is_operator(char *input)
{
	int	i;

	i = 0;
	while(input[i])
	{
		if (input[i] == '|' || input[i] == '<' || input[i] == '>'
			|| input[i] == '(' || input[i] == ')' || input[i] == '&')
			return (1);
		i++;
	}
	return (0);
}
int	ending_quotes(char *input, int i);

t_token	*create_token(t_token_type token_type, char *value);

t_token	*add_token(t_token **token_list, t_token_type token_type, char *value);

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
		if (c == isspace(input[i]))
			add_token(token_list, SPACE, " ");
		else if (c == '|')
			add_token(token_list, PIPE, "|");
		else if (c == '<')
		{
			if (input[i + 1] == '<')
				add_token(token_list, APPEND, "<<");
			add_token(token_list, REDIRECT_IN, "<");
		}
		else if (c == '>')
		{
			if (input[i + 1] == '>')
				add_token(token_list, HEREDOC, ">>");
			add_token(token_list, REDIRECT_OUT, ">");
		}
		else if (c == '\0')
			add_token(token_list, EOF, NULL);
		else
			tokenize_word(input);
		{
			if (c == '"' || c == '\'')
			{
				end_word = ending_quotes(input, i);
				word_value = ft_substr(input, i + 1, end_word - i - 1);
				add_token(token_list, WORD, word_value);
				i = end_word;
			}
			else
			{
				start_word = i;
				while (i < len_input && !(isspace(&input[i]) || is_operator(&input[i])))
					i++;
				word_value = ft_substr(input, start_word, i - start_word);
				add_token(token_list, WORD, word_value);
			}
		}
		i++;
	}
	return (token_list);
}
 
t_token *free_tokens(t_token *list);







// t_token	*tokenize_word(char *input)
// {
// 	t_token	*token_list;
// 	int		i;
// 	int		len_input;
// 	char	c;
// 	int		start_word;
// 	int		end_word;
// 	char	*word_value;

// 	if (c == '"' || c == '\'')
// 	{
// 		end_word = find_end_of_quotes(input, i);
// 		word_value = ft_substr(input, i + 1, end_word - 1);
// 		add_token(token_list, WORD, word_value);
// 		i = end_word;
// 	}
// 	else
// 	{
// 		start_word = i;
// 		while (i < len_input && !(isspace(&input[i]) || is_operator(&input[i])))
// 			i++;
// 		word_value = ft_substr(input, start_word, i);
// 		add_token(token_list, WORD, word_value);
// 	}
// }
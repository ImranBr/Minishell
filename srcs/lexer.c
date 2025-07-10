/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:00:47 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/06 20:32:49 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

int	tokenize_operators(char *input, t_token **token_list, int i)
{
	if (input[i] == '|')
		add_token(token_list, create_token(PIPE, "|"));
	if (input[i] == '<')
	{
		if (input[i + 1] == '<')
		{
			add_token(token_list, create_token(HEREDOC, "<<"));
			i++;
		}
		else
			add_token(token_list, create_token(REDIRECT_IN, "<"));
	}
	else if (input[i] == '>')
	{
		if (input[i + 1] == '>')
		{
			add_token(token_list, create_token(APPEND, ">>"));
			i++;
		}
		else
			add_token(token_list, create_token(REDIRECT_OUT, ">"));
	}
	return (i);
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

int tokenize_words(char *input, t_token **token_list, size_t start_index)
{
    size_t i = start_index;
    char *assembled_word = malloc(100000); // prévoir une taille suffisante
    int pos = 0; // position dans assembled_word

    while (i < strlen(input) && !is_space(input[i]) && input[i] != '|' && input[i] != '<' && input[i] != '>')
    {
        if (input[i] == '\'' || input[i] == '"')
        {
            char quote = input[i];
            i++;
            while (i < strlen(input) && input[i] != quote)
            {
                assembled_word[pos++] = input[i++];
            }
            if (input[i] == quote)
                i++;
        }
        else
        {
            assembled_word[pos++] = input[i++];
        }
    }
    assembled_word[pos] = '\0';

    add_token(token_list, create_token(WORD, assembled_word));
    free(assembled_word); // si create_token fait strdup

    return i - 1; // retourne l'index du dernier caractère traité
}

// int	tokenize_words(char *input, t_token **token_list, int i)
// {
// 	char	*word_value;
// 	int		start;
// 	int		end_word;

// 	start = i;
// 	if (input[i] == '"' || input[i] == '\'')
// 	{
// 		end_word = ending_quotes(input, i);
// 		word_value = ft_substr(input, i, end_word - i + 1);
// 		i = end_word;
// 	}
// 	else
// 	{
// 		while (input[i] && !is_space(input[i]) && !is_operator(input[i]))
// 			i++;
// 		word_value = ft_substr(input, start, i - start);
// 		i--;
// 	}
// 	add_token(token_list, create_token(WORD, word_value));
// 	free(word_value);
// 	return (i);
// }

t_token	*tokenize(char *input)
{
	t_token	*token_list;
	int		i;
	int		len_input;

	token_list = NULL;
	i = 0;
	len_input = ft_strlen(input);
	while (i < len_input)
	{
		if (is_space(input[i]))
		{
			i++;
			continue ;
		}
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			i = tokenize_operators(input, &token_list, i);
		else
			i = tokenize_words(input, &token_list, i);
		i++;
	}
	add_token(&token_list, create_token(EOF_TOKEN, NULL));
	return (token_list);
}

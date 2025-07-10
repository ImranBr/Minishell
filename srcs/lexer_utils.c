/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 23:16:42 by joudafke          #+#    #+#             */
/*   Updated: 2025/07/08 15:59:53 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

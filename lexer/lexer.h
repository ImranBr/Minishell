/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joudafke <joudafke@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:07:31 by joudafke          #+#    #+#             */
/*   Updated: 2025/06/12 21:11:30 by joudafke         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>

typedef enum e_token_type
{
	SPACE,
	WORD,
	PIPE,
	REDIRECT_IN,
	APPEND,
	REDIRECT_OUT,
	HEREDOC,
	EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}					t_token;

#endif
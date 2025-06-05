/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:19:51 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/05 07:44:48 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "printf.h"
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

            //builtin
int		builtin_echo(char **args);
int		builtin_env(char **env);
int		builtin_pwd(void);
// int		builtin_cd(char **args);

            // caracteres
int		are_double_quotes_closed(char *input);
int		are_single_quotes_closed(char *input);
void	neutralize_special_char_in_double_quote(char *input);
void	neutralize_special_char_in_single_quote(char *input);
            // syntax
void	syntax_pipe(char *input);
#endif
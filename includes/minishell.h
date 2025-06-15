/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:19:51 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/06/15 20:32:33 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "printf.h"
# include <errno.h>
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_data
{
	t_env			*env;
}					t_data;
// structure qui comporte:

// list = type de variable pour contenir
//          ex. char **, linked list avec char * dedans ou quoi

// structure qui comporte une list des commande qui soit une structure elle meme
// qui comporte tous ce dont elle a besoin
//

// minishell: ls -l -o | wc -l | prout -1

// struc->cmdName == ls
// struc->args[0] == -l
// struc->args[1] == -o
// struc = struc->next
// struc->cmdName == wc
// struc->args[0] == -l
// struc->args[1] == existe pas
// struc = struc->next
// struc->cmdName == prout

// expand
// char				*get_env_value(char *name, t_env *env_list);
t_env				*create_env_list(char **envp);

// builtin
int					builtin_echo(char **args);
int					builtin_env(t_env *env_list);
int					builtin_pwd(void);
void				ft_cd(char *path);
void				exec_builtin(char **args, t_env *env_list);
int					builtin_export(t_env *env_list, char **cmd);

// caracteres
int					are_double_quotes_closed(char *input);
int					are_single_quotes_closed(char *input);
int					is_special_char(char c);
void				neutralize_special_char_in_double_quote(char *input);
void				neutralize_special_char_in_single_quote(char *input);

// syntax
void				syntax_special_char(char *input);

// utils
int					ft_strcmp(char *s1, char *s2);
t_env	*free_list(t_env *a);

#endif
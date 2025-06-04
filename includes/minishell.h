#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "printf.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <linux/limits.h>

int		builtin_env(char **env);
int		builtin_pwd(void);
void	syntax_pipe(char *input);
int		builtin_echo(char **args);
// int		builtin_cd(char **args);
int are_double_quotes_closed(char *input);
int are_single_quotes_closed(char *input);

#endif
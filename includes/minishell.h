#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "printf.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int		builtin_env(char **env);
int		builtin_pwd(void);
void	syntax_pipe(char *input);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibarbouc <ibarbouc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:23:08 by ibarbouc          #+#    #+#             */
/*   Updated: 2025/07/10 15:23:47 by ibarbouc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp( char *s1,  char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n && s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
/*int	main(void)
{
	char	*a1 = "salut";
	char	*a2 = "Salut";

	printf("%d\n", ft_strncmp(a1, a2, 20));
}*/

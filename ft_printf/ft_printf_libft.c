/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_libft.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frrusso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/23 11:27:50 by frrusso           #+#    #+#             */
/*   Updated: 2022/01/23 11:27:54 by frrusso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_isneg(int n)
{
	if (n < 0)
		return (1);
	else
		return (0);
}

int	ft_dsize(unsigned int d)
{
	int	i;

	if (d == 0)
		return (1);
	i = 1;
	while (d > 9)
	{
		d = d / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int				i;
	int				neg;
	unsigned int	num;
	char			*res;

	i = ft_isneg(n);
	neg = ft_isneg(n);
	if (n < 0)
		num = -n;
	else
		num = n;
	i += ft_dsize(num);
	res = malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	res[i] = '\0';
	while (i > 0 + neg)
	{
		i--;
		res[i] = '0' + (num % 10);
		num = num / 10;
	}
	if (neg)
		res[0] = '-';
	return (res);
}

char	*ft_utoa(unsigned int n)
{
	int		i;
	char	*res;

	i = ft_dsize(n);
	res = malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	res[i] = '\0';
	while (i > 0)
	{
		i--;
		res[i] = '0' + (n % 10);
		n = n / 10;
	}
	return (res);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

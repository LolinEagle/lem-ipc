/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frrusso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 13:03:37 by frrusso           #+#    #+#             */
/*   Updated: 2022/01/21 13:03:43 by frrusso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_conversion(char str, va_list list)
{
	int	i;

	if (str == 'c')
		ft_putchar_fd((char)va_arg(list, int), 1);
	else if (str == 's')
		i = ft_printf_put((char *)va_arg(list, char *));
	else if (str == 'p')
		i = ft_printf_ptr((long unsigned int)va_arg(list, long unsigned int));
	else if (str == 'd')
		i = ft_printf_int(list);
	else if (str == 'i')
		i = ft_printf_int(list);
	else if (str == 'u')
		i = ft_printf_uns(list);
	else if (str == 'x')
		i = ft_printf_hex((unsigned int)va_arg(list, unsigned int), str);
	else if (str == 'X')
		i = ft_printf_hex((unsigned int)va_arg(list, unsigned int), str);
	else if (str == '%')
		ft_putchar_fd('%', 1);
	else
		return (-1);
	if (str == 'c' || str == '%')
		i = 1;
	return (i);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	int		j;
	va_list	list;

	i = 0;
	va_start(list, str);
	while (*str)
	{
		if (*str != '%')
		{
			ft_putchar_fd(*str, 1);
			i++;
		}
		else
		{
			str++;
			j = ft_conversion(*str, list);
			if (j < 0)
				continue ;
			i += j;
		}
		str++;
	}
	va_end(list);
	return (i);
}

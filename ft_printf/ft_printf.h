/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frrusso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 13:03:49 by frrusso           #+#    #+#             */
/*   Updated: 2022/01/23 15:05:32 by frrusso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>// write
# include <stdlib.h>// malloc free
# include <stdarg.h>// va_start va_arg va_copy va_end

char	*ft_itoa(int n);
char	*ft_utoa(unsigned int n);
void	ft_putchar_fd(char c, int fd);
int		ft_printf_put(char *s);
int		ft_printf_int(va_list list);
int		ft_printf_hex(long unsigned int hex, char c);
int		ft_printf_ptr(long unsigned int ptr);
int		ft_printf_uns(va_list list);
int		ft_printf(const char *str, ...);

#endif

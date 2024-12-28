/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaferna2 <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:22:41 by jaferna2          #+#    #+#             */
/*   Updated: 2024/10/23 10:46:48 by jaferna2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# ifndef HEXA
#  define HEXA "0123456789abcdef"
# endif

# ifndef HEXA_UP
#  define HEXA_UP "0123456789ABCDEF"
# endif

# ifndef DEC
#  define DEC "0123456789"
# endif

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <stdio.h>

int		ft_printf(int fd, char const *args, ...);
int		ft_putchar(int fd, char c);
int		ft_putstr(int fd, char *c);
int		ft_putnbr_base(int fd, int n, char *base);
int		ft_putnbr_hex(int fd, int n, char *base);
int		ft_put_pointr(int fd, void *n, int flag);

#endif /*FT_PRINTF_H*/

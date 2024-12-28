/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaferna2 <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:22:10 by jaferna2          #+#    #+#             */
/*   Updated: 2024/10/23 10:46:41 by jaferna2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

int	ft_putchar(int fd, char c)
{
	return (write(fd, &c, 1));
}

int	ft_putstr(int fd, char *s)
{
	int		i;

	i = 0;
	if (s == NULL)
		s = "(null)";
	while (s[i])
		i += write(fd, &s[i], 1);
	return (i);
}

int	ft_putnbr_base(int fd, int n, char *base)
{
	int		len_base;
	int		count;
	char	temp_n;

	count = 0;
	len_base = 0;
	while (base[len_base])
		len_base++;
	if (n < 0)
	{
		write (fd, "-", 1);
		count++;
		if (n == -2147483647 - 1)
		{
			write(fd, "2147483648", 10);
			return (11);
		}
		n = -n;
	}
	if (n >= len_base)
		count += ft_putnbr_base(fd, n / len_base, base);
	temp_n = base[(n % len_base)];
	write(fd, &temp_n, 1);
	count++;
	return (count);
}

int	ft_putnbr_hex(int fd, int n, char *base)
{
	int				len_base;
	int				count;
	char			temp_n;
	unsigned long	temp_un;

	count = 0;
	len_base = 0;
	while (base[len_base])
		len_base++;
	if (n < 0)
		temp_un = (unsigned int) n;
	else
		temp_un = n;
	if (temp_un >= len_base)
		count += ft_putnbr_hex(fd, temp_un / len_base, base);
	temp_n = base[(temp_un % len_base)];
	write(fd, &temp_n, 1);
	count++;
	return (count);
}

int	ft_put_pointr(int fd, void *n, int flag)
{
	int					len_base;
	int					count;
	char				temp_n;
	unsigned long long	address;

	count = 0;
	if (n == NULL)
		return (ft_putstr(1, "(nil)"));
	if (flag)
		count += ft_putstr(1, "0x");
	len_base = 0;
	while (HEXA[len_base])
		len_base++;
	address = (unsigned long long)n;
	if ((unsigned long long)n >= len_base)
		count += ft_put_pointr(fd, (void *)(address / len_base), 0);
	temp_n = HEXA[((unsigned long long) n % len_base)];
	write(fd, &temp_n, 1);
	count++;
	return (count);
}

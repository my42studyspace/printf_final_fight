/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstein <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 19:28:55 by pstein            #+#    #+#             */
/*   Updated: 2019/10/22 13:16:08 by pstein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
** width functions. wneh str = NULL we have special function :)
*/

static void		width_next(char **str, t_flags *f, int len)
{
	char	*new_s;

	if (!(new_s = ft_strnew(len)))
		return ;
	if (f->zero && !f->minus && !(f->precision > -1
				&& (f->conv == 'd' || f->conv == 'i' || f->conv == 'o'
					|| f->conv == 'u' || f->conv == 'i' || f->conv == 'x'
					|| f->conv == 'X' || f->conv == 'b')))
		ft_memset(new_s, '0', len);
	else
		ft_memset(new_s, ' ', len);
	if (f->minus)
		*str = ft_strplus(*str, new_s, 1, 1);
	else
	{
		if (*str && (*str[0] == '+' || *str[0] == '-' || *str[0] == ' ')
				&& f->zero)
			*str = ft_sets1_inside(new_s, *str, 1);
		else if (f->zero && ((f->conv == 'p')
					|| ((f->conv == 'x' || f->conv == 'X')
					&& f->hash)))
			*str = ft_sets1_inside(new_s, *str, 2);
		else
			*str = ft_strplus(new_s, *str, 1, 1);
	}
}

void			set_width(char **str, t_flags *flags)
{
	int	len;

	if (flags->width < 0)
	{
		flags->width *= -1;
		flags->minus = 1;
	}
	len = flags->width - ft_strlen(*str);
	if (flags->width && len > 0)
		width_next(str, flags, len);
}

static int		print_null(char *str, char b[4096], int *i, t_flags *flags)
{
	int j;

	j = 1;
	if (str)
		free(str);
	j += print_buff(b, i);
	if (flags->flag_width == 1 && flags->width > 0)
	{
		while (--(flags->width) && j++)
			write(1, flags->zero ? "0" : " ", 1);
	}
	if (flags->conv == 'c' || flags->conv == 'C')
		write(1, "\0", 1);
	if (flags->flag_width == 1 && flags->width < 0)
	{
		flags->width *= -1;
		while (--(flags->width) && j++)
			write(1, flags->zero ? "0" : " ", 1);
	}
	return (j);
}

int				lets_do_rock(va_list *va, char **s, char b[4096], int *i)
{
	t_flags	flags;
	char	*str_ret;
	int		ret;

	if ((ret = all_flags(&flags, s, va)) == -1)
		return (0);
	flags.conv = **s;
	if (ret == 0)
	{
		if (!(str_ret = ft_memset(ft_strnew(1), **s, 1)))
			return (0);
	}
	else
		str_ret = conv_checker(&flags, va);
	if (flags.null)
		return (print_null(str_ret, b, i, &flags));
	else if (!str_ret && (flags.conv == 'C'
				|| flags.conv == 'S' || (flags.type == l
					&& (flags.conv == 'c' || flags.conv == 's'))))
		return (-1);
	else if (str_ret)
		set_width(&str_ret, &flags);
	if (!str_ret)
		return (0);
	return (print_buff(b, i) + ft_putstr_free(str_ret));
}

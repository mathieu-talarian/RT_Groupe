/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jubarbie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/19 14:55:18 by jubarbie          #+#    #+#             */
/*   Updated: 2016/12/14 18:10:56 by jubarbie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		quit_rt(t_env *e)
{
	ft_putstr("\033[32mQuiting rt\033[0m\n");
	free_env(e);
	exit(EXIT_SUCCESS);
	return (0);
}

void	error_perso(t_env *e, char *str)
{
	ft_putstr("\033[31mError: ");
	ft_putstr(str);
	ft_putstr("\033[0m\n");
	free(e);
	exit(EXIT_FAILURE);
}

void	error_file(t_env *e)
{
	ft_putstr("\033[31mWrong file format\033[0m\n");
	free(e);
	exit(EXIT_FAILURE);
}

void	error_usage(void)
{
	ft_putstr("usage: ./rt [-");
	ft_putstr(OPT_REF);
	ft_putstr("] file_name\n");
	exit(EXIT_FAILURE);
}

void	error_opt(char opt)
{
	ft_putstr("./rt: illegal option -- ");
	ft_putchar(opt);
	ft_putchar('\n');
	error_usage();
}

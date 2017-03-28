/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_color_for_tex.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoullec <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/30 16:12:58 by mmoullec          #+#    #+#             */
/*   Updated: 2017/02/05 20:05:28 by mmoullec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		modify_color_for_tex(char *tex, t_v3d vec, t_light *datas, double coef)
{
	datas->shadow = 0.0;
	if (tex)
	{
		if (!ft_strcmp(tex, "wood"))
		{
			datas->rgb = wood(vec, coef);
			return (1);
		}
		if (!ft_strcmp(tex, "marbre"))
		{
			datas->rgb = marbre(vec, coef);
			return (1);
		}
		if (!ft_strcmp(tex, "random"))
		{
			datas->rgb = random_noise(coef, vec, datas->rgb);
			return (1);
		}
		if (!ft_strcmp(tex, "damier"))
		{
			datas->rgb = damier(coef, vec, datas->rgb);
			return (1);
		}
	}
	return (0);
}

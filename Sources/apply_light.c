/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoullec <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/02 10:29:13 by mmoullec          #+#    #+#             */
/*   Updated: 2017/02/07 17:56:19 by atoulous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int		init_light_ray(t_param *param, t_object *light)
{
	t_v3d	tmp;

	PHO_RAY.pos = light->pos;
	tmp = sub_v3d(VW_RAY.inter, PHO_RAY.pos);
	PHO_RAY.dist = length_v3d(tmp);
	PHO_RAY.dir = unit_v3d(tmp);
	PHO_RAY.obj = NULL;
	if (light->angle && (dot_v3d(PHO_RAY.dir, light->dir) < cos(light->angle)))
		return (0);
	return (1);
}

static t_rgb	transfo(unsigned int col, t_light *datas)
{
	t_hsv	h;

	rgb_to_hsv(col, &h.h, &h.s, &h.v);
	datas->shadow = 0.0;
	return (my_hsv_to_rgb(h));
}

static void		obj_sel(t_light *datas, t_param *param)
{
	t_object *obj_sel;

	datas->ray = 1;
	if (param->e->scene->obj_focus)
	{
		obj_sel = (t_object *)param->e->scene->obj_focus->content;
		if (VW_RAY.obj == obj_sel)
			datas->ray = 0;
	}
	if (PHO_RAY.obj)
		datas->shadow -= param->AMBIANCE;
}

static void		apply_light2(t_param *param, t_list *lst_obj, t_light *datas)
{
	if (cos_v3d(PHO_RAY.dir, VW_RAY.norm) < 0.00001)
	{
		lst_obj = param->e->scene->obj;
		while (lst_obj)
		{
			datas->obj = (t_object *)lst_obj->content;
			if (datas->obj != VW_RAY.obj)
				(*(param->e->obj_fct_obj[datas->obj->type]))(param->e, \
					datas->obj, &PHO_RAY, &SOL);
			lst_obj = lst_obj->next;
		}
	}
	obj_sel(datas, param);
	!(param->OPT & CRTN) ? apply_color(param->e, param, datas) : 0;
	(param->OPT & CRTN) ? apply_cartoon_color(param->e, param, datas) : 0;
	datas->hsv = my_rgb_to_hsv(datas->rgb);
	COLOR = hsv_to_rgb(datas->hsv.h, datas->hsv.s, datas->hsv.v + \
			datas->shadow);
}

void			apply_light(t_env *e, t_param *param)
{
	t_list		*lst_light;
	t_light		datas;

	datas.rgb = transfo(VW_RAY.obj->color, &datas);
	VW_RAY.obj->pro ? modify_color_for_tex(VW_RAY.obj->pro, \
		sub_v3d(VW_RAY.inter, VW_RAY.obj->pos), &datas, VW_RAY.obj->coef) : 0;
	IS_PHONG ? rgb_s_mult(&datas.rgb, VW_RAY.obj->mat.ambient + LUMI) : 0;
	lst_light = e->scene->light;
	while (lst_light)
	{
		if (init_light_ray(param, lst_light->content))
			apply_light2(param, lst_light->content, &datas);
		else
		{
			datas.hsv = my_rgb_to_hsv(datas.rgb);
			COLOR = hsv_to_rgb(datas.hsv.h, datas.hsv.s, AMBIANCE * LUMI);
		}
		lst_light = lst_light->next;
	}
}

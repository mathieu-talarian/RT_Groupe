/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jubarbie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/20 16:58:17 by jubarbie          #+#    #+#             */
/*   Updated: 2017/02/07 18:08:59 by atoulous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		set_cylinder_norm(t_object *obj, t_ray *ray)
{
	double	l;

	ray->inter = add_v3d(ray->pos, smul_v3d(ray->dir, ray->dist));
	l = length_v3d(sub_v3d(ray->inter, O_POS));
	l = sqrt(pow(l, 2.0) - pow(O_R1, 2.0));
	ray->norm = unit_v3d(sub_v3d(ray->inter, add_v3d(O_POS,
					smul_v3d(O_DIR, l))));
	if (obj->asp && (!ft_strcmp(obj->asp, "bump")))
		modif_normale(obj->density, 4, &ray->norm, \
				sub_v3d(ray->inter, obj->pos));
}

static void		find_dist(t_object *obj, t_ray *ray, t_sol *sol)
{
	int		i;
	double	max;
	int		index;

	i = -1;
	max = ray->dist;
	while (++i < 4)
		if (T[i] > 0.000001 && T[i] < max)
		{
			index = i;
			max = T[i];
		}
	if (max < ray->dist)
	{
		ray->dist = max;
		ray->obj = obj;
		if (index == 0 || index == 1)
			set_cylinder_norm(obj, ray);
		else if (index == 2)
			ray->norm = O_DIR;
		else if (index == 3)
			ray->norm = smul_v3d(O_DIR, -1);
	}
}

static void		find_solutions(t_object *obj, t_ray *ray, t_sol *sol)
{
	t_v3d	tmp;

	if ((DET = ft_solve_quadratic(A, B, C, T)) >= 0)
	{
		T[2] = -1;
		while (++T[2] < 2)
			if (T[(int)T[2]] >= 0)
			{
				tmp = add_v3d(ray->pos, smul_v3d(ray->dir, T[(int)T[2]]));
				if (!(dot_v3d(O_DIR, sub_v3d(tmp, O_POS)) > 0 &&
							dot_v3d(O_DIR, sub_v3d(tmp, O_P2)) < 0))
					T[(int)T[2]] = -1;
			}
		T[2] = caps(ray, O_R1, O_DIR, O_P2);
		T[3] = caps(ray, O_R1, smul_v3d(O_DIR, -1), O_POS);
		find_dist(obj, ray, sol);
	}
}

void			cylinder(t_env *e, t_object *obj, t_ray *ray, t_sol *sol)
{
	t_v3d	dp;
	t_v3d	tmp;
	int		i;

	i = e->opt;
	dp = sub_v3d(ray->pos, O_POS);
	tmp = sub_v3d(ray->dir, smul_v3d(O_DIR, dot_v3d(ray->dir, O_DIR)));
	A = dot_v3d(tmp, tmp);
	B = 2 * dot_v3d((sub_v3d(ray->dir, smul_v3d(O_DIR,
		dot_v3d(ray->dir, O_DIR)))), sub_v3d(dp, \
			smul_v3d(O_DIR, dot_v3d(dp, O_DIR))));
	tmp = sub_v3d(dp, smul_v3d(O_DIR, dot_v3d(dp, O_DIR)));
	C = dot_v3d(tmp, tmp) - pow(O_R1, 2.0);
	find_solutions(obj, ray, sol);
}

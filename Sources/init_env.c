/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jubarbie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/19 15:06:39 by jubarbie          #+#    #+#             */
/*   Updated: 2017/02/07 15:48:45 by jubarbie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** Building the scene by calling the parser and computing all cam parameters
*/

static void		init_scene(t_env *e, char *file_name)
{
	parse_rt(e, file_name);
	e->parse_cam_pos = CAM_POS;
	e->parse_cam_dir = CAM_DIR;
	IS_OPT_H ? print_help() : 0;
	VW_WIDTH = IMG_WIDTH / 1000.0;
	VW_HEIGHT = IMG_HEIGHT / 1000.0;
	VW_DIST = 1.0;
	GAP_X = VW_WIDTH / (double)IMG_WIDTH;
	GAP_Y = VW_HEIGHT / (double)IMG_HEIGHT;
	CAM_DIR = unit_v3d(CAM_DIR);
	CAM_UP = v3d(0, 1, 0);
	if (CAM_DIR.y == fmax(fmax(CAM_DIR.x, CAM_DIR.y), CAM_DIR.z))
		CAM_UP = v3d(0, 0, 1);
	if (CAM_DIR.y == fmin(fmin(CAM_DIR.x, CAM_DIR.y), CAM_DIR.z) &&
																CAM_DIR.y < 0)
		CAM_UP = v3d(0, 0, -1);
	CAM_RIGHT = cross_v3d(CAM_UP, CAM_DIR);
	VW_DIST = 1.0;
	CAM_UP = cross_v3d(CAM_DIR, CAM_RIGHT);
}

/*
** Initialize a param structure
** Called for each thread
*/

static t_param	*init_param(t_env *e, int index)
{
	t_param	*param;

	if (!(param = malloc(sizeof(t_param))))
		error_perso(e, "malloc (t_param *)param failed");
	ENV = e;
	TH = index;
	return (param);
}

static void		init_img(t_env *e)
{
	WIN_WIDTH = (IMG_WIDTH < 600) ? 840 : IMG_WIDTH + 240;
	WIN_HEIGHT = (IMG_HEIGHT < 666) ? 846 : IMG_HEIGHT + 180;
	IMG_GAP_X = (WIN_WIDTH - 240 - IMG_WIDTH) / 2;
	IMG_GAP_Y = (WIN_HEIGHT - 180 - IMG_HEIGHT) / 2;
	IMG = mlx_new_image(MLX, IMG_WIDTH, IMG_HEIGHT);
	IMG_ADDR = mlx_get_data_addr(IMG, &e->img.bpp, &e->img.sizeline, &ENDIAN);
}

/*
** Initialize the environment. Should only be called once at program launch
*/

t_env			*init_env(char *file_name, char opt)
{
	t_env		*e;
	int			i;

	if (!(e = (t_env *)malloc(sizeof(t_env))))
		error_perso(e, "malloc (t_env *)e failed");
	if (ft_strcmp(file_name + ft_strlen(file_name) - 3, ".rt"))
		error_perso(e, "Bad file extension (.rt)");
	init_opt(e, opt);
	init_scene(e, file_name);
	i = -1;
	while (++i < NB_TH)
		e->param[i] = init_param(e, i);
	MLX = mlx_init();
	init_img(e);
	WIN = mlx_new_window(MLX, WIN_WIDTH, WIN_HEIGHT, file_name);
	e->scene->obj_focus = NULL;
	e->scene->obj_trash = NULL;
	init_menu(e);
	debug(e);
	LUMI = 0.5;
	SPEED = 0.2;
	ALPHA_ROT = 20;
	return (e);
}

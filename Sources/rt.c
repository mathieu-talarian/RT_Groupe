/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jubarbie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/19 13:01:24 by jubarbie          #+#    #+#             */
/*   Updated: 2017/02/06 23:44:39 by jubarbie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** Calling the raytracer function for each thread
** Display the image on the window
*/

int			create_img(t_env *e)
{
	int			i;
	pthread_t	th[NB_TH];

	if (IS_STEREO)
		stereo(e);
	else
	{
		i = -1;
		while (++i < NB_TH)
			if (pthread_create(&th[i], NULL, &raytracer,
													(void *)(e->param[i])) < 0)
				error_perso(e, "create thread failed");
		i = -1;
		while (++i < NB_TH)
			(void)pthread_join(th[i], NULL);
		mlx_put_image_to_window(MLX, WIN, IMG, IMG_GAP_X, IMG_GAP_Y + 49);
	}
	return (0);
}

/*
** Save the color in the pixel (x, y) of the image given
*/

void		img_put_pixel(t_img *img, int x, int y, t_param *param)
{
	t_env	*e;
	char	*addr;

	e = ENV;
	addr = img->addr;
	if (IS_SEPIA)
		sepia_filter(param);
	else if (IS_GREY)
		grey_filter(param);
	addr[y * img->sizeline + x * (img->bpp / 8)] = fmin(F_COLOR.r, 255.0);
	addr[y * img->sizeline + x * (img->bpp / 8) + 1] = fmin(F_COLOR.g, 255.0);
	addr[y * img->sizeline + x * (img->bpp / 8) + 2] = fmin(F_COLOR.b, 255.0);
	if (IS_MOTION_BLUR)
		img->addr[y * img->sizeline + x * (img->bpp / 8) + 3] = 150;
	else
		img->addr[y * img->sizeline + x * (img->bpp / 8) + 3] = 0;
}

int			main(int ac, char **av)
{
	t_env	*e;
	int		opt;
	int		i;

	if (ac - (i = get_options(ac, av, &opt)) <= 1)
		error_usage();
	e = init_env(av[i + 1], opt);
	create_img(e);
	mlx_loop_hook(MLX, moves, e);
	mlx_hook(WIN, 17, Button1MotionMask, quit_rt, e);
	mlx_hook(WIN, KeyPress, KeyPressMask, ft_key_press, e);
	mlx_hook(WIN, KeyRelease, KeyReleaseMask, ft_key_release, e);
	mlx_hook(WIN, ButtonPress, ButtonPressMask, ft_mouse_click, e);
	mlx_loop(MLX);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   hooks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/20 22:53:42 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/20 23:10:43 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	hook(void *param)
{
	t_fdf	*data;

	data = param;
	draw_vec(data->map, data);
}

void	scrollhook(double xdelta, double ydelta, void *param)
{
	t_fdf	*data;

	data = param;
	if (ydelta > 0)
		data->z_scale -= 0.05;
	if (ydelta < 0)
		data->z_scale += 0.05;
}

void	resize(int32_t width, int32_t height, void *param)
{
	t_fdf	*data;
	int32_t	old_x;
	int32_t	old_y;

	data = param;
	ft_bzero(data->img->pixels, data->img->width
		* data->img->height * sizeof(uint32_t));
	old_x = data->img->width;
	old_y = data->img->height;
	mlx_resize_image(data->img, width, height);
	data->transpose[X] += (width / 2) - (old_x / 2);
	data->transpose[Y] += (height / 2) - (old_y / 2);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   draw_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/20 20:32:40 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/20 21:41:38 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

float	find_max(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

bool	vec_inbounds(t_fdf *fdf, t_vec_point a)
{
	if (!((int)a.vec[X] < fdf->img->width && (int)a.vec[Y] < fdf->img->height
			&& (int)a.vec[X] > 0 && (int)a.vec[Y] > 0))
	{
		return (false);
	}
	if (fdf->img->pixels[((int)a.vec[Y] * fdf->img->width
				+ (int)a.vec[X]) * sizeof(int32_t)] != NULL)
	{
		return (false);
	}
	return (true);
}

uint32_t	interpolate(uint32_t color1, uint32_t color2, float fraction)
{
	t_interpolate	col;

	col.r1 = (color1 >> 24) & 0xff;
	col.r2 = (color2 >> 24) & 0xff;
	col.g1 = (color1 >> 16) & 0xff;
	col.g2 = (color2 >> 16) & 0xff;
	col.b1 = (color1 >> 8) & 0xff;
	col.b2 = (color2 >> 8) & 0xff;
	col.a1 = color1 & 0xff;
	col.a2 = color2 & 0xff;
	return ((int)((col.r2 - col.r1) * fraction + col.r1) << 24 |
			(int)((col.g2 - col.g1) * fraction + col.g1) << 16 |
			(int)((col.b2 - col.b1) * fraction + col.b1) << 8 |
			(int)((col.a2 - col.a1) * fraction + col.a1));
}

uint32_t	interpol_vec_col(t_vec_point *a, t_vec_point *b, t_fdf *data)
{
	u_int32_t	col;

	if (data->colour != true)
		return (a->colour);
	data->settings->fraction = fabs((a->vec[Y] - data->settings->start_y)
			/ (b->vec[Y] - data->settings->start_y));
	col = interpolate(a->colour, b->colour, data->settings->fraction);
	return (col);
}

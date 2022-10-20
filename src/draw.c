/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   draw.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/02 22:05:26 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/20 20:28:16 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

float	find_max(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

void	rotate(t_vec_point *og, t_fdf *data)
{
	t_vec_point	rotated;
	t_vec		r;

	r = (t_vec){data->x_angle + data->angle, data->y_angle
		+ data->angle, data->z_angle + data->angle, 1};
	rotated.vec[X] = og->vec[X] * (cos(r[Z]) * cos(r[Y])) + \
		og->vec[Y] * (cos(r[Z]) * sin(r[Y]) * \
		sin(r[X]) - sin(r[Z]) * cos(r[X])) + \
		og->vec[Z] * (cos(r[Z]) * sin(r[Y]) * \
		cos(r[X]) + sin(r[Z]) * sin(r[X]));
	rotated.vec[Y] = og->vec[X] * (sin(r[Z]) * cos(r[Y])) + \
		og->vec[Y] * (sin(r[Z]) * sin(r[Y]) * \
		sin(r[X]) + cos(r[Z]) * cos(r[X])) + \
		og->vec[Z] * (sin(r[Z]) * sin(r[Y]) * \
		cos(r[X]) - cos(r[Z]) * sin(r[X]));
	rotated.vec[Z] = og->vec[X] * (-sin(r[Y])) + \
		og->vec[Y] * (cos(r[Y]) * sin(r[X])) + \
		og->vec[Z] * (cos(r[Y]) * cos(r[X]));
	rotated.colour = og->colour;
	*og = rotated;
}

void	project(t_vec_point *original, t_fdf *data)
{
	t_vec_point	projected;
	double	depth;

	depth = 1 / (data->distance - original->vec[Z]);
	if (depth <= 0)
		depth = 1;
	projected.vec[X] = original->vec[X] * depth;
	projected.vec[Y] = original->vec[Y] * depth;
	projected.vec[Z] = original->vec[Z] * depth;
	projected.colour = original->colour;
	*original = projected;
}

void	subtract(t_vec_point *original, t_fdf *data)
{
	t_vec_point	subtracted;

	subtracted.vec[X] = original->vec[X] - (data->width / 2);
	subtracted.vec[Y] = original->vec[Y] - (data->height / 2);
	if (data->sub_z == true)
		subtracted.vec[Z] = original->vec[Z] - (data->z_max / 2);
	else
		subtracted.vec[Z] = original->vec[Z];
	subtracted.colour = original->colour;
	*original = subtracted;
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

void	vec_matrixes(t_vec_point *point, t_fdf *data)
{
	t_vec		scale;
	t_transform	*settings;

	settings = data->settings;
	scale = (t_vec){settings->scale, settings->scale, settings->scale};
	subtract(point, data);
	point->vec[Z] *= data->z_scale;
	rotate(point, data);
	if (data->perspective == true)
		project(point, data);
	point->vec *= scale;
	point->vec += data->transpose;
}

void	apply_vec_transform(t_vec_point *a, t_vec_point *b, t_fdf *fdf)
{
	t_transform	*settings;
	t_vec		scale;

	settings = fdf->settings;
	scale = (t_vec){settings->scale, settings->scale, settings->scale};
	settings->fraction = 0;
	settings->a_z = a->vec[Z];
	settings->b_z = b->vec[Z];
	vec_matrixes(a, fdf);
	vec_matrixes(b, fdf);
	settings->start_x = a->vec[X];
	settings->start_y = a->vec[Y];
	settings->step_x = b->vec[X] - a->vec[X];
	settings->step_y = b->vec[Y] - a->vec[Y];
	settings->max = find_max(fabs(settings->step_x),
			fabs(settings->step_y));
	settings->step_x /= settings->max;
	settings->step_y /= settings->max;
}

bool	vec_inbounds(t_fdf *fdf, t_vec_point a)
{
	bool	ret;

	ret = true;
	if (!((int)a.vec[X] < fdf->img->width && (int)a.vec[Y] < fdf->img->height
			&& (int)a.vec[X] > 0 && (int)a.vec[Y] > 0))
	{
		ret = false;
	}
	if (fdf->img->pixels[((int)a.vec[Y] * fdf->img->width + (int)a.vec[X]) * sizeof(int32_t)] != NULL)
	{
		ret = false;
	}
	return (ret);
}

void	bresenham_vec(t_vec_point a, t_vec_point b, t_fdf *data)
{
	uint32_t	colour;

	apply_vec_transform(&a, &b, data);
	if ((a.vec[X] >= data->img->width && b.vec[X] >= data->img->width)
		|| (a.vec[X] <= 0 && b.vec[X] <= 0))
		return ;
	if ((a.vec[Y] >= data->img->height && b.vec[Y] >= data->img->height)
		|| (a.vec[Y] <= 0 && b.vec[Y] <= 0))
		return ;
	while ((int)(a.vec[X] - b.vec[X]) || (int)(a.vec[Y] - b.vec[Y]))
	{
		colour = interpol_vec_col(&a, &b, data);
		if (vec_inbounds(data, a))
			mlx_put_pixel(data->img, (int)(a.vec[X]), (int)(a.vec[Y]), colour);
		a.vec[X] += data->settings->step_x;
		a.vec[Y] += data->settings->step_y;
	}
	if (data->colour == true)
		colour = interpol_vec_col(&a, &b, data);
	if (vec_inbounds(data, a))
		mlx_put_pixel(data->img, (int)a.vec[X], (int)a.vec[Y], colour);
}

void	draw_vec(t_vec_point **matrix, t_fdf *fdf)
{
	int	x;
	int	y;

	y = 0;
	ft_bzero(fdf->img->pixels, fdf->img->width
		* fdf->img->height * sizeof(uint32_t));
	while (y < fdf->height)
	{
		x = 0;
		while (x < fdf->width)
		{
			if (x + 1 < fdf->width)
				bresenham_vec(matrix[y][x], matrix[y][x + 1], fdf);
			if (y + 1 < fdf->height)
				bresenham_vec(matrix[y][x], matrix[y + 1][x], fdf);
			if (fdf->triangles == 1)
				if (x + 1 < fdf->width && y + 1 < fdf->height)
					bresenham_vec(matrix[y][x], matrix[y + 1][x + 1], fdf);
			x++;
		}
		y++;
	}
	fdf->frames += 1;
}

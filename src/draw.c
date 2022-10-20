/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   draw.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/02 22:05:26 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/20 20:35:01 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

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

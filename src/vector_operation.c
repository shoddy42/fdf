/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_operation.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/20 20:33:08 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/20 23:13:42 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

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
	double		depth;

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

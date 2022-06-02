	/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   draw.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/02 22:05:26 by wkonings      #+#    #+#                 */
/*   Updated: 2022/05/09 21:31:08 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

float	ft_abs(float nb)
{
	// depricated, as fabs is a builtin i can use.
	// might need again if fabs using doubles causes issues.
	if (nb < 0)
		return (nb * -1);
	return (nb);
}

float	find_max(float a, float b)
{
	// self explanatory function.
	if (a > b)
		return (a);
	return (b);
}

void	rotate(t_vec_point *og, t_fdf *data)
{
	t_vec rot = {data->x_angle + data->angle, data->y_angle + data->angle, data->z_angle + data->angle, 1};
	t_vec_point rotated;

	rotated.vec[X] = og->vec[X] * (cos(rot[Z]) * cos(rot[Y])) + 
		og->vec[Y] * (cos(rot[Z]) * sin(rot[Y]) * sin(rot[X]) - sin(rot[Z]) * cos(rot[X])) +
		og->vec[Z] * (cos(rot[Z]) * sin(rot[Y]) * cos(rot[X]) + sin(rot[Z]) * sin(rot[X]));
	rotated.vec[Y] = og->vec[X] * (sin(rot[Z]) * cos(rot[Y])) +
		og->vec[Y] * (sin(rot[Z]) * sin(rot[Y]) * sin(rot[X]) + cos(rot[Z]) * cos(rot[X])) +
		og->vec[Z] * (sin(rot[Z]) * sin(rot[Y]) * cos(rot[X]) - cos(rot[Z]) * sin(rot[X]));
	rotated.vec[Z] = og->vec[X] * (- sin(rot[Y])) +
		og->vec[Y] * (cos(rot[Y]) * sin(rot[X])) +
		og->vec[Z] * (cos(rot[Y]) * cos(rot[X]));
	rotated.colour = og->colour;
	*og = rotated;
}

void	rotateX(t_point *original, double angle)
{
	t_point	rotated;
	rotated.x = original->x;
	rotated.y = (original->y * cos(angle)) + (original->z * -sin(angle));
	rotated.z = (original->y * sin(angle)) + (original->z * cos(angle));
	// printf("x:%f y:%f z:%f\n", rotated.x, rotated.y, rotated.z);
	*original = rotated;
}

void	rotateY(t_point *original, double angle)
{
	t_point	rotated;
	rotated.x = (original->x * cos(angle)) + (original->z * sin(angle));
	rotated.y = original->y;
	rotated.z = (original->x * -sin(angle)) + (original->z * cos(angle));
	// printf("x:%f y:%f z:%f\n", rotated.x, rotated.y, rotated.z);
	*original = rotated;
}

void	rotateZ(t_point *original, double angle)
{
	t_point	rotated;

	rotated.x = (original->x * cos(angle)) + (original->y * -sin(angle));
	rotated.y = (original->x * sin(angle)) + (original->y * cos(angle));
	rotated.z = original->z;
	// printf("x:%f y:%f z:%f\n", rotated.x, rotated.y, rotated.z);
	*original = rotated;
}

void	project(t_point *original, t_fdf *data)
{
	//applies actual perspective.
	t_point projected;
	double depth;

	depth = 1 / (data->distance - original->z);
	// if (depth <= 0)
	// 	depth = 1;
	projected.x = original->x * depth;
	projected.y = original->y * depth;
	projected.z = original->z * depth;
	*original = projected;
}

void	subtract(t_point *original, t_fdf *data)
{
	//function should be finished.
	t_point subtracted;

	subtracted.x = original->x - (data->width / 2);
	subtracted.y = original->y - (data->height / 2);
	if (data->sub_z == true)
		subtracted.z = original->z - (data->z_max / 2);
	else
		subtracted.z = original->z;
	*original = subtracted;
}

uint32_t interpolate(uint32_t color1, uint32_t color2, float fraction)
{
        unsigned char   r1 = (color1 >> 24) & 0xff;
        unsigned char   r2 = (color2 >> 24) & 0xff;
        unsigned char   g1 = (color1 >> 16) & 0xff;
        unsigned char   g2 = (color2 >> 16) & 0xff;
        unsigned char   b1 = (color1 >> 8) & 0xff;
        unsigned char   b2 = (color2 >> 8) & 0xff;
		unsigned char	a1 = color1 & 0xff;
		unsigned char	a2 = color2 & 0xff;

        return ((int) ((r2 - r1) * fraction + r1) << 24 |
                (int) ((g2 - g1) * fraction + g1) << 16 |
                (int) ((b2 - b1) * fraction + b1) << 8|
				(int) ((a2 - a1) * fraction + a1));
}

uint32_t interpol_col(t_point *a, t_point *b, t_fdf *data)
{
	t_point *tmp;
	u_int32_t col;

	if (data->colour != true)
		return (a->colour);
	if (a->x == data->settings->start_x)
		return (a->colour);
	if (a->x == b->x)
		return (b->colour);
	data->settings->f_step = fabs(a->x / b->x);
	data->settings->fraction = data->settings->f_step;
	// printf("frac = %f\n", data->settings->fraction);
	col = interpolate(a->colour, b->colour, 0.5);
	// col = interpolate(a->colour, b->colour, data->settings->fraction);
	return(col);
}

uint32_t interpol_vec_col(t_vec_point *a, t_vec_point *b, t_fdf *data)
{
	t_point *tmp;
	u_int32_t col;

	if (data->colour != true)
		return(a->colour);
	data->settings->f_step = fabs(a->vec[X] / b->vec[X]);
	data->settings->fraction = data->settings->f_step;
	// printf("frac = %f\n", data->settings->fraction);
	// col = interpolate(a->colour, b->colour, 0.9);
	col = interpolate(a->colour, b->colour, data->settings->fraction);
	return(col);
}

void	vec_matrixes(t_vec_point *point, t_fdf *data)
{
	t_vec scale = {data->settings->scale, data->settings->scale, data->settings->scale};

	subtract(point, data);
	point->vec[Z] *= data->z_scale;
	// rotate(point, data);
	rotateX(point, data->angle + data->x_angle);
	rotateY(point, data->angle + data->y_angle);
	rotateZ(point, data->angle + data->z_angle);
	if (data->perspective == true)
		project(point, data);
	point->vec *= scale;
	point->vec += data->transpose;
}

void	apply_vec_transform(t_vec_point *a, t_vec_point *b, t_fdf *fdf)
{
	t_vec scale = {fdf->settings->scale, fdf->settings->scale, fdf->settings->scale};

	fdf->settings->fraction = 0;
	fdf->settings->a_z = a->vec[Z];
	fdf->settings->b_z = b->vec[Z];
	fdf->settings->start_x = a->vec[X];
	fdf->settings->start_y = a->vec[Y];
	vec_matrixes(a, fdf);
	vec_matrixes(b, fdf);
	fdf->settings->step_x = b->vec[X] - a->vec[X];
	fdf->settings->step_y = b->vec[Y] - a->vec[Y];
	fdf->settings->max = find_max(fabs(fdf->settings->step_x), fabs(fdf->settings->step_y));
	fdf->settings->step_x /= fdf->settings->max;
	fdf->settings->step_y /= fdf->settings->max;

}

bool	vec_inbounds(t_fdf *fdf, t_vec_point a)
{
	// printf("cp = x: %f y: %f converted = x: %i y: %i\n", a.x, a.y, (int)a.x, (int)a.y);
	if ((int)a.vec[X] < fdf->img->width && (int)a.vec[Y] < fdf->img->height && (int)a.vec[X] > 0 && (int)a.vec[Y] > 0)
		return (true);
	return (false);
}

void	bresenham_vec(t_vec_point a, t_vec_point b, t_fdf *data)
{
	uint32_t colour;

	apply_vec_transform(&a, &b, data);
	if ((a.vec[X] >= data->img->width && b.vec[X] >= data->img->width) || (a.vec[X] <= 0 && b.vec[X] <= 0))
		return ;
	if ((a.vec[Y] >= data->img->height && b.vec[Y] >= data->img->height) || (a.vec[Y] <= 0 && b.vec[Y] <= 0))
		return ;
	while ((int)(a.vec[X] - b.vec[X]) || (int)(a.vec[Y] - b.vec[Y]))
	{
		colour = interpol_vec_col(&a, &b, data);
		if (vec_inbounds(data, a))
			mlx_put_pixel(data->img, (int)(a.vec[X]), (int)(a.vec[Y]), colour);
		a.vec[X] += data->settings->step_x;
		a.vec[Y] += data->settings->step_y;
	}
	// if (data->colour == true)
	colour = interpol_col(&a, &b, data);
	if (vec_inbounds(data, a))
		mlx_put_pixel(data->img, (int)a.vec[X], (int)a.vec[Y], colour);
} 

void	draw_vec(t_vec_point **matrix, t_fdf *fdf)
{
	int	x;
	int	y;

	y = 0;
	ft_bzero(fdf->img->pixels, fdf->img->width * fdf->img->height * sizeof(uint32_t));
	while (y < fdf->height)
	{
		x = 0;
		while (x < fdf->width)
		{
			if (x + 1 < fdf->width )
				bresenham_vec(matrix[y][x], matrix[y][x + 1], fdf);
			if (y + 1 < fdf->height)
				bresenham_vec(matrix[y][x], matrix[y + 1][x], fdf);
			if (fdf->triangles == 1)
				if (x + 1 < fdf->width && y + 1 < fdf->height)
					bresenham_vec(matrix[y][x], matrix[y + 1][x + 1], fdf); // diagonal
			x++;
		}
		y++;
	}
	fdf->frames += 1;
}

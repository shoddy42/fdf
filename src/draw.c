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

// void	isometric(t_point *point, double angle)
// {
// 	//depricated. replaced by rotateX, -Y, -Z.
// 	point->x = (point->x - point->y) * cos(angle);
// 	point->y = (point->x + point->y) * sin(angle) - point->z;
// }

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
	double z;

	z = 1 / (data->distance - original->z);
	if (z <= 0)
		z = 0.1;
	projected.x = original->x * z;
	projected.y = original->y * z;
	projected.z = original->z * z;
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

// uint32_t	get_col(t_point *a, t_point *b, t_fdf *data)
// {
// 	// this is garbage. want to make it gradient. colours should be applied only once, preferably during creation.
// 	t_gradient	col;
// 	t_point		tmp;

// 	col.water = 0x0000FFFF;
// 	col.sand  =	0xFFFF00FF;
// 	col.grass =	0x00FF00FF;
// 	col.rock  = 0x555555FF;
// 	col.snow  = 0xFFFFFFFF;

// 	// if (!a->colour)
// 	// 	printf("joebiden\n");
// 	if (b->z > a->z)
// 		tmp.z = b->z;
// 	else
// 		tmp.z = a->z;
// 	if ((int)tmp.z >= data->z_max)
// 		return (col.snow);
// 	if ((int)tmp.z >= data->z_max * 0.75)
// 		return (col.rock);
// 	if ((int)tmp.z >= data->z_max * 0.5)
// 		return (col.grass);
// 	if ((int)tmp.z >= data->z_max * 0.25)
// 		return (col.sand);
// 	return (col.water);
// }

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
		return(a->colour);
	data->settings->f_step = fabs(data->settings->start_x - a->x / (b->x - data->settings->start_x));
	data->settings->fraction = data->settings->f_step;
	// printf("frac = %f\n", data->settings->fraction);
	col = interpolate(a->colour, b->colour, data->settings->fraction);
	return(col);
}

uint32_t interpol_vec_col(t_vec_point *a, t_vec_point *b, t_fdf *data)
{
	t_point *tmp;
	u_int32_t col;

	if (data->colour != true)
		return(a->colour);
	data->settings->f_step = fabs(data->settings->start_x - a->vec[X] / (b->vec[X] - data->settings->start_x));
	data->settings->fraction = data->settings->f_step;
	// printf("frac = %f\n", data->settings->fraction);
	col = interpolate(a->colour, b->colour, data->settings->fraction);
	return(col);
}


void	apply_transform(t_point *a, t_point *b, t_fdf *fdf)
{
	// a->colour = interpol_col(a, b, fdf);
	fdf->settings->fraction = 0;
	fdf->settings->a_z = a->z;
	fdf->settings->b_z = b->z;
	fdf->settings->start_x = a->x;
	fdf->settings->start_y = a->y;
	subtract(a, fdf);
	subtract(b, fdf);
	// APPLY SCALE
	a->z *= fdf->z_scale;
	b->z *= fdf->z_scale;
	// printf("colour = [%u]\n", a->colour);
	// APPLY ROTATION
	rotateX(a, fdf->angle + fdf->x_angle);
	rotateX(b, fdf->angle + fdf->x_angle);
	rotateY(a, fdf->angle + fdf->y_angle);
	rotateY(b, fdf->angle + fdf->y_angle);
	rotateZ(a, fdf->angle + fdf->z_angle);
	rotateZ(b, fdf->angle + fdf->z_angle);
	// APPLY PROJECTION
	if (fdf->perspective == true)
	{
		project(a, fdf);
		project(b, fdf);
	}
	// APPLY SCALE
	a->x *= fdf->settings->scale;
	a->y *= fdf->settings->scale;
	b->x *= fdf->settings->scale;
	b->y *= fdf->settings->scale;
	// APPLY SHIFT
	a->x += fdf->settings->shift_x;
	a->y += fdf->settings->shift_y;
	b->x += fdf->settings->shift_x;
	b->y += fdf->settings->shift_y;
	// BEEP BOOP
	fdf->settings->step_x = b->x - a->x;
	fdf->settings->step_y = b->y - a->y;
	fdf->settings->max = find_max(fabs(fdf->settings->step_x), fabs(fdf->settings->step_y));
	fdf->settings->step_x /= fdf->settings->max;
	fdf->settings->step_y /= fdf->settings->max;
}

void	apply_vec_transform(t_vec_point *a, t_vec_point *b, t_fdf *fdf)
{
	t_vec scale = {fdf->settings->scale, fdf->settings->scale, fdf->settings->scale};

	fdf->settings->fraction = 0;
	fdf->settings->a_z = a->vec[Z];
	fdf->settings->b_z = b->vec[Z];
	fdf->settings->start_x = a->vec[X];
	fdf->settings->start_y = a->vec[Y];
	subtract(a, fdf);
	subtract(b, fdf);
	// APPLY SCALE
	a->vec[Z] *= fdf->z_scale;
	b->vec[Z] *= fdf->z_scale;
	// printf("colour = [%u]\n", a->colour);
	// APPLY ROTATION
	rotate(a, fdf);
	rotate(b, fdf);
	// APPLY PROJECTION
	if (fdf->perspective == true)
	{
		project(a, fdf);
		project(b, fdf);
	}
	// APPLY SCALE
	a->vec *= scale;
	b->vec *= scale;
	// APPLY SHIFT
	a->vec[X] += fdf->settings->shift_x;
	a->vec[Y] += fdf->settings->shift_y;
	b->vec[X] += fdf->settings->shift_x;
	b->vec[Y] += fdf->settings->shift_y;
	// BEEP BOOP
	fdf->settings->step_x = b->vec[X] - a->vec[X];
	fdf->settings->step_y = b->vec[Y] - a->vec[Y];
	fdf->settings->max = find_max(fabs(fdf->settings->step_x), fabs(fdf->settings->step_y));
	fdf->settings->step_x /= fdf->settings->max;
	fdf->settings->step_y /= fdf->settings->max;
}

bool	inbounds(t_fdf *fdf, t_point a)
{
	// printf("cp = x: %f y: %f converted = x: %i y: %i\n", a.x, a.y, (int)a.x, (int)a.y);
	if ((int)a.x < fdf->img->width && (int)a.y < fdf->img->height && (int)a.x > 0 && (int)a.y > 0)
		return (true);
	return (false);
}

bool	vec_inbounds(t_fdf *fdf, t_vec_point a)
{
	// printf("cp = x: %f y: %f converted = x: %i y: %i\n", a.x, a.y, (int)a.x, (int)a.y);
	if ((int)a.vec[X] < fdf->img->width && (int)a.vec[Y] < fdf->img->height && (int)a.vec[X] > 0 && (int)a.vec[Y] > 0)
		return (true);
	return (false);
}


void	bresenham_p(t_point a, t_point b, t_fdf *data)
{
	uint32_t colour;

	apply_transform(&a, &b, data);
	if ((a.x >= data->img->width && b.x >= data->img->width) || (a.x <= 0 && b.x <= 0))
		return ;
	if ((a.y >= data->img->height && b.y >= data->img->height) || (a.y <= 0 && b.y <= 0))
		return ;
	while ((int)(a.x - b.x) || (int)(a.y - b.y))
	{
		colour = interpol_col(&a, &b, data);
		if (inbounds(data, a))
			mlx_put_pixel(data->img, (int)(a.x), (int)(a.y), colour);
		a.x += data->settings->step_x;
		a.y += data->settings->step_y;
	}
	if (data->colour == true)
		a.colour = interpol_col(&a, &b, data);
	if (inbounds(data, a))
		mlx_put_pixel(data->img, (int)a.x, (int)a.y, colour);
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
		// a.colour = interpol_col(&a, &b, data);
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


void	draw(t_point **matrix, t_fdf *fdf)
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
				bresenham_p(matrix[y][x], matrix[y][x + 1], fdf);
			if (y + 1 < fdf->height)
				bresenham_p(matrix[y][x], matrix[y + 1][x], fdf);
			if (fdf->triangles == 1)
				if (x + 1 < fdf->width && y + 1 < fdf->height)
					bresenham_p(matrix[y][x], matrix[y + 1][x + 1], fdf); // diagonal
			x++;
		}
		y++;
	}
	fdf->frames += 1;
}

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
	if (nb < 0)
		return (nb * -1);
	return (nb);
}

float	find_max(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

void	isometric(t_point *point, double angle)
{
	point->x = (point->x - point->y) * cos(angle);
	point->y = (point->x + point->y) * sin(angle) - point->z;
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
	rotated.z = (original->x * -sin(angle));// + (original->z * cos(angle));
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
	t_point projected;
	double z;

	// printf("dist: %f oz: %f cal: %f\n", data->distance, original->z, 1 / (data->distance - original->z));
	z = 1 / (data->distance - original->z);

	projected.x = original->x * z;
	projected.y = original->y * z;
	projected.z = original->z * z;
	*original = projected;
}

void	subtract(t_point *original, t_fdf *data)
{
	t_point subtracted;

	subtracted.x = original->x - (data->width / 2);
	subtracted.y = original->y - (data->height / 2);
	subtracted.z = original->z;
	*original = subtracted;
}

void	apply_transform(t_point *a, t_point *b, t_fdf *fdf)
{
	// isometric(a, fdf->angle);
	// isometric(b, fdf->angle);

	// APPLY SCALE
	// APPLY ROTATION
	a->z *= fdf->z_scale;
	b->z *= fdf->z_scale;
	subtract(a, fdf);
	subtract(b, fdf);
	rotateX(a, fdf->angle + fdf->x_angle);
	rotateX(b, fdf->angle + fdf->x_angle);
	rotateY(a, fdf->angle + fdf->y_angle);
	rotateY(b, fdf->angle + fdf->y_angle);
	rotateZ(a, fdf->angle + fdf->z_angle);
	rotateZ(b, fdf->angle + fdf->z_angle);
	// APPLY PROJECTION
	project(a, fdf);
	project(b, fdf);
	// apply_perspective(a, fdf);
	// apply_perspective(b, fdf);
	// APPLY SHIFT
	a->x *= fdf->settings->scale;
	a->y *= fdf->settings->scale;
	b->x *= fdf->settings->scale;
	b->y *= fdf->settings->scale;
	a->x += fdf->settings->shift_x;
	a->y += fdf->settings->shift_y;
	b->x += fdf->settings->shift_x;
	b->y += fdf->settings->shift_y;
	fdf->settings->step_x = b->x - a->x;
	fdf->settings->step_y = b->y - a->y;
	fdf->settings->max = find_max(fabs(fdf->settings->step_x), fabs(fdf->settings->step_y));
	fdf->settings->step_x /= fdf->settings->max;
	fdf->settings->step_y /= fdf->settings->max;
	a->colour = 0xffff00ff;
	// a->colour = 0x0000000fff; // black
	// if (a->z > 0)
	//  a->colour = 0xfffffff;
}

bool	inbounds(t_fdf *fdf, t_point a)
{
	// printf("cp = x: %f y: %f converted = x: %i y: %i\n", a.x, a.y, (int)a.x, (int)a.y);
	if ((int)a.x <= fdf->img->width && (int)a.y <= fdf->img->width && (int)a.x > 0 && (int)a.y > 0)
		return (true);
	return (false);
}

void	bresenham_p(t_point a, t_point b, t_fdf *fdf, char *tst)
{
	apply_transform(&a, &b, fdf);
	// printf("calc= a.x: %f a.y: %f ax-bx: %f, ay-by: %f angle: %f\n", round(a.x), round(a.y), round(a.x - b.x), round(a.y - b.y), fdf->angle);
	while (round(a.x - b.x) || round(a.y - b.y))
	{
		if (inbounds(fdf, a))
			mlx_put_pixel(fdf->img, round(a.x), round(a.y), a.colour);
		// printf("%s a.x: %f a.y: %f xstep: %f ystep: %f\n", tst, a.x, a.y, fdf->settings->step_x, fdf->settings->step_y);
		a.x += fdf->settings->step_x;
		a.y += fdf->settings->step_y;
		// a.x = fabs(a.x);
		// if (a.x >= fdf->mlx->width || a.y >= fdf->mlx->height || a.x < 0 || a.y < 0)
		// 	break;
	}
	if (inbounds(fdf, a))
		mlx_put_pixel(fdf->img, (int)a.x, (int)a.y, a.colour);
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
			if (x + 1 < fdf->width)
				bresenham_p(matrix[y][x], matrix[y][x + 1], fdf, "x line");
			if (y + 1 < fdf->height)
				bresenham_p(matrix[y][x], matrix[y + 1][x], fdf, "y line");
			// if (x + 1 < fdf->width && y + 1 < fdf->height)
			// 	bresenham_p(matrix[y][x], matrix[y + 1][x + 1], fdf); // diagonal
			x++;
		}
		y++;
	}
}

// void	draw_cube(t_fdf *fdf)
// {
// 	t_point **cube;

// 	ft_bzero(fdf->img->pixels, fdf->img->width * fdf->img->height * sizeof(uint32_t));
// 	cube = scuffed_vec();

// 	bresenham_p(cube[0][0], cube[1][0], fdf);
// 	bresenham_p(cube[1][0], cube[2][0], fdf);
// 	bresenham_p(cube[2][0], cube[3][0], fdf);
// 	bresenham_p(cube[3][0], cube[0][0], fdf);

// 	bresenham_p(cube[0][0], cube[4][0], fdf);
// 	bresenham_p(cube[1][0], cube[5][0], fdf);
// 	bresenham_p(cube[2][0], cube[6][0], fdf);
// 	bresenham_p(cube[3][0], cube[7][0], fdf);

// 	bresenham_p(cube[4][0], cube[5][0], fdf);
// 	bresenham_p(cube[5][0], cube[6][0], fdf);
// 	bresenham_p(cube[6][0], cube[7][0], fdf);
// 	bresenham_p(cube[7][0], cube[4][0], fdf);


// 	// bresenham_p(cube[7][0], cube[0][0], fdf);
// }

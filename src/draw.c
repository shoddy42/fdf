/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   draw.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/02 22:05:26 by wkonings      #+#    #+#                 */
/*   Updated: 2022/05/07 00:29:38 by wkonings      ########   odam.nl         */
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
	else
		return (b);
}

// void	apply_transform(t_point *a, t_point *b, t_fdf *fdf)
// {
// 	a.x *= fdf->settings->scale;
// 	a.y *= fdf->settings->scale;
// 	b.x *= fdf->settings->scale;
// 	b.y *= fdf->settings->scale;
// 	// x = (x - y) * cos(angle);
// 	// y = (x + y) * sin(angle) - 2;
// 	a.x += fdf->settings->shift_x;
// 	a.y += fdf->settings->shift_y;
// 	b.x += fdf->settings->shift_x;
// 	b.y += fdf->settings->shift_y;
// 	printf("sx %f, sy %f\n", fdf->settings->shift_x, fdf->settings->shift_y);
// 	fdf->settings->step_x = b.x - a.x;
// 	fdf->settings->step_y = b.y - a.y;
// 	fdf->settings->max = find_max(ft_abs(fdf->settings->step_x), ft_abs(fdf->settings->step_y));
// 	fdf->settings->step_x /= fdf->settings->max;
// 	fdf->settings->step_y /= fdf->settings->max;
// }

void	isometric(t_point *point)
{
	// printf("pre  point: x: %f y: %f z: %f\n", point->x, point->y, point->z);
	point->x = (point->x - point->y) * cos((double)0.8);
	if (point->x + point->y - point->z < 0)
		printf("bad\n");
	point->y = (point->x + point->y) * sin((double)0.8) - point->z;
	// printf("past point: x: %f y: %f z: %f\n", point->x, point->y, point->z);
}

void	apply_transform(t_point *a, t_point *b, t_fdf *fdf)
{
	a->x *= fdf->settings->scale;
	a->y *= fdf->settings->scale;
	b->x *= fdf->settings->scale;
	b->y *= fdf->settings->scale;
	// printf("pre pre a: x: %f y: %f z: %f\n", a->x, a->y, a->z);
	// printf("pre pre b: x: %f y: %f z: %f\n", b->x, b->y, b->z);
	// printf("a go in\n");
	isometric(a);
	// printf("b go in\n");
	isometric(b);
	// printf("DONE a: x: %f y: %f z: %f\n", a->x, a->y, a->z);
	// printf("DONE b: x: %f y: %f z: %f\n", b->x, b->y, b->z);
	a->x += fdf->settings->shift_x;
	a->y += fdf->settings->shift_y;
	b->x += fdf->settings->shift_x;
	b->y += fdf->settings->shift_y;
	// printf("sx %f, sy %f\n", fdf->settings->shift_x, fdf->settings->shift_y);
	fdf->settings->step_x = b->x - a->x;
	fdf->settings->step_y = b->y - a->y;
	fdf->settings->max = find_max(ft_abs(fdf->settings->step_x), ft_abs(fdf->settings->step_y));
	fdf->settings->step_x /= fdf->settings->max;
	fdf->settings->step_y /= fdf->settings->max;
}

void	bresenham_p(t_point a, t_point b, t_fdf *fdf, uint32_t color)
{
	// printf("OMNI BEFORE a: x: %f y: %f z: %f\n", a.x, a.y, a.z);
	// printf("OMNI BEFORE b: x: %f y: %f z: %f\n", b.x, b.y, b.z);
	apply_transform(&a, &b, fdf);
	while ((int)(a.x - b.x) || (int)(a.y - b.y))
	{
		printf ("a\n");
		mlx_put_pixel(fdf->img, (int)a.x, (int)a.y, color);
		a.x += fdf->settings->step_x;
		a.y += fdf->settings->step_y;
		if (a.x >= fdf->mlx->width || a.y >= fdf->mlx->height || a.x < 0 || a.y < 0)
			break;
	}
} 

// void	bresenham(float x, float y, float x1, float y1, t_fdf *fdf, uint32_t color)
// {
// 	// int i = 0;

// 	// printf ("bruhsenham: x: %f y: %f x1: %f y1: %f\n", x, y, x1, y1);
// 	// MOVE BLOCK TO APPLY_TRANSFORM
// 	x *=  fdf->settings->scale;
// 	y *=  fdf->settings->scale;
// 	x1 *= fdf->settings->scale;
// 	y1 *= fdf->settings->scale;
// 	x += fdf->settings->shift_x;
// 	y += fdf->settings->shift_y;
// 	x1 += fdf->settings->shift_x;
// 	y1 += fdf->settings->shift_y;
// 	printf("sx %f, sy %f\n", fdf->settings->shift_x, fdf->settings->shift_y);
// 	fdf->settings->step_x = x1 - x;
// 	fdf->settings->step_y = y1 - y;
// 	fdf->settings->max = find_max(ft_abs(fdf->settings->step_x), ft_abs(fdf->settings->step_y));
// 	fdf->settings->step_x /= fdf->settings->max;
// 	fdf->settings->step_y /= fdf->settings->max;

// 	while ((int)(x - x1) || (int)(y - y1))
// 	{
// 		mlx_put_pixel(fdf->img, x, y, color);
// 		// if (i < 100)
// 			// printf ("bruh: x = %f, y = %f ?? a:%i b:%i\n", x, y, (int)(x - x1), (int)(y - y1));
// 		x += fdf->settings->step_x;
// 		y += fdf->settings->step_y;
// 		// i++;
// 		if (x >= fdf->mlx->width || y >= fdf->mlx->height || x < 0 || y < 0)
// 			break;
// 	}
// }

void	draw(t_point **matrix, t_fdf *fdf)
{
	int		y;
	int		x;

	// print_menu(PRM);
	
	uint32_t color = 0xfffffff;
	y = 0;
	while (y < fdf->height)
	{
		// i = 0;
		x = 0;
		printf("x: %i y %i, w: %i h: %i\n", x ,y, fdf->width, fdf->height);
		while (x < fdf->width)
		{
			// printf("OMEGA OMNI OMNI BEFORE a: x: %f y: %f z: %f\n", matrix[y][x].x, matrix[y][x].y, matrix[y][x].z);
			// if (x + 1 < fdf->width)
			// 	bresenham(x, y, x + 1, y, fdf, color);
			// if (y + 1 < fdf->height)
			// 	bresenham(x, y, x, y + 1, fdf, color);
			if (x + 1 < fdf->width)
				bresenham_p(matrix[y][x], matrix[y][x + 1], fdf, color);
			if (y + 1 < fdf->height)
				bresenham_p(matrix[y][x], matrix[y + 1][x], fdf, color);
			// bresenham(x, y, x + 1, y + 1, fdf, color); // diagonal
			x++;
		}
		printf("\n");
		y++;
	}
	// bresenham(fdf->width,y,fdf->width,y + 1,fdf,color + 9000);
}

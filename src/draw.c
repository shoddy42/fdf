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

void	isometric(t_point *point, float angle)
{
	point->x = (point->x - point->y) * cos(angle);
	point->y = (point->x + point->y) * sin(angle) - point->z;
}

void	apply_transform(t_point *a, t_point *b, t_fdf *fdf)
{
	a->x *= fdf->settings->scale;
	a->y *= fdf->settings->scale;
	b->x *= fdf->settings->scale;
	b->y *= fdf->settings->scale;
	isometric(a, fdf->angle);
	isometric(b, fdf->angle);
	a->x += fdf->settings->shift_x;
	a->y += fdf->settings->shift_y;
	b->x += fdf->settings->shift_x;
	b->y += fdf->settings->shift_y;
	fdf->settings->step_x = b->x - a->x;
	fdf->settings->step_y = b->y - a->y;
	fdf->settings->max = find_max(ft_abs(fdf->settings->step_x), ft_abs(fdf->settings->step_y));
	fdf->settings->step_x /= fdf->settings->max;
	fdf->settings->step_y /= fdf->settings->max;
	a->colour = 0xfEEEEfff;
	if (a->z > 0)
	 a->colour = 0xfffffff;
}

bool	inbounds(t_fdf *fdf, t_point a)
{
	if ((int)a.x <= fdf->img->width && (int)a.y <= fdf->img->width)
		return (true);
	return (false);
}

void	bresenham_p(t_point a, t_point b, t_fdf *fdf)
{
	apply_transform(&a, &b, fdf);
	while ((int)(a.x - b.x) || (int)(a.y - b.y))
	{
		if (inbounds(fdf, a))
			mlx_put_pixel(fdf->img, (int)a.x, (int)a.y, a.colour);
		a.x += fdf->settings->step_x;
		a.y += fdf->settings->step_y;
		if (a.x >= fdf->mlx->width || a.y >= fdf->mlx->height || a.x < 0 || a.y < 0)
			break;
	}
} 

void	draw(t_point **matrix, t_fdf *fdf)
{
	int		x;
	int		y;
	
	uint32_t color = 0xfffffff;
	y = 0;
	ft_bzero(fdf->img->pixels, fdf->img->width * fdf->img->height * sizeof(uint32_t));
	while (y < fdf->height)
	{
		x = 0;
		while (x < fdf->width)
		{
			if (x + 1 < fdf->width)
				bresenham_p(matrix[y][x], matrix[y][x + 1], fdf);
			if (y + 1 < fdf->height)
				bresenham_p(matrix[y][x], matrix[y + 1][x], fdf);
			// if (x + 1 < fdf->width && y + 1 < fdf->height)
			// 	bresenham_p(matrix[y][x], matrix[y + 1][x + 1], fdf); // diagonal
			x++;
		}
		y++;
	}
}

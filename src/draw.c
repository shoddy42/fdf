/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   draw.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/02 22:05:26 by wkonings      #+#    #+#                 */
/*   Updated: 2022/05/06 17:25:12 by wkonings      ########   odam.nl         */
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

void	set_params()
{




	
}

void	bresenham(float x, float y, float x1, float y1, t_fdf *fdf, uint32_t color)
{
	float	step_x;
	float	step_y;
	float	max;
	int i = 0;

	// printf ("bruhsenham: x: %f y: %f x1: %f y1: %f\n", x, y, x1, y1);
	x *= fdf->scale;
	y *= fdf->scale;
	x1 *= fdf->scale;
	y1 *= fdf->scale;
	step_x = x1 - x;
	step_y = y1 - y;
	// printf ("post scale: x = %f, y = %f, x1: %f, y1: %f, step_x: %f, step_y: %f\n", x, y, x1, y1, step_x, step_y);
	max = find_max(ft_abs(step_x), ft_abs(step_y));
	step_x /= max;
	step_y /= max;
	while ((int)(x - x1) || (int)(y - y1))
	{
		mlx_put_pixel(fdf->img, x, y, color);
		if (i < 100)
			// printf ("bruh: x = %f, y = %f ?? a:%i b:%i\n", x, y, (int)(x - x1), (int)(y - y1));
		x += step_x;
		y += step_y;
		i++;
		if (x >= fdf->mlx->width || y >= fdf->mlx->height || x < 0 || y < 0)
			break;
	}
}

void	draw(int **matrix, t_fdf *fdf)
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
			if (x + 1 < fdf->width)
				bresenham(x, y, x + 1, y, fdf, color);
			if (y + 1 < fdf->height)
				bresenham(x, y, x, y + 1, fdf, color);
			// bresenham(x, y, x + 1, y + 1, fdf, color); // diagonal
			x++;
		}
		printf("\n");
		y++;
	}
	bresenham(fdf->width,y,fdf->width,y + 1,fdf,color + 9000);
}

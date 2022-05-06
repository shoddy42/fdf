/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 21:16:22 by wkonings      #+#    #+#                 */
/*   Updated: 2022/05/04 20:46:45 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	square(t_fdf *fdf, int color)
{
	int	x;
	int	y;

	x = 500;
	y = 500;
	while (y <= 1000)
	{
		mlx_put_pixel(fdf->img, x, y, color);
		x = 500;
		while (x <= 1000)
		{
			mlx_put_pixel(fdf->img, x, y, color);
			x++;
		}
		y++;
	}
}

void my_keyhook(mlx_key_data_t keydata, void* param)
{
	if (mlx_is_key_down(param, MLX_KEY_ESCAPE))
		mlx_close_window(param);
	if (keydata.key == MLX_KEY_J && keydata.action == MLX_PRESS)
		puts("Hello ");
	if (keydata.key == MLX_KEY_K && keydata.action == MLX_RELEASE)
		puts("World");
	if (keydata.key == MLX_KEY_L && keydata.action == MLX_REPEAT)
		puts("!");
}

void init_struct(t_fdf *fdf, char **av)
{
	int i;

	i = 0;
	fdf->width = 0;
	fdf->height = 0;
	fdf->error = 0;
	fdf->scale = 30;
	get_height(fdf, av[1]);
	fdf->map = calloc(sizeof(int *), fdf->height);
	if (!fdf->map)
		exit(92);
	i = -1;
	while (++i < fdf->height)
		fdf->map[i] = calloc(sizeof(int), fdf->width);
	fill_map(fdf, av[1]);
	fdf->mlx = mlx_init(1000, 1000, "FdF", true);
	if (!fdf->mlx)
		exit(1);
	fdf->img = mlx_new_image(fdf->mlx, 1920, 1080);
	mlx_key_hook(fdf->mlx, &my_keyhook, fdf->mlx);
	mlx_image_to_window(fdf->mlx, fdf->img, 0 ,0);
}

int	main(int ac, char **av)
{
	t_fdf		*fdf;	
	fdf = malloc(sizeof(t_fdf));
	if (ac != 2)
	{
		printf ("bad input\n");
		return (12);
	}
	init_struct(fdf, av);
	// bresenham2(1, 0, 1, 1, fdf);
	draw(fdf->map, fdf);
	// bresenham(9, 10, 10, 10, fdf);
	// bresenham(0, 0, 1000, 600, fdf);
	// square(fdf, 0xFF00FF);
	mlx_loop(fdf->mlx);
	mlx_delete_image(fdf->mlx, fdf->img);
	mlx_terminate(fdf->mlx);
	return (0);
}

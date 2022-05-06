/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 21:16:22 by wkonings      #+#    #+#                 */
/*   Updated: 2022/05/07 00:07:53 by wkonings      ########   odam.nl         */
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
	t_fdf *fdf;

	fdf = param;

	if (mlx_is_key_down(fdf->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(fdf->mlx);
	if (keydata.key == MLX_KEY_UP && keydata.action == MLX_PRESS)
		fdf->settings->shift_y -= 1;
	if (keydata.key == MLX_KEY_DOWN && keydata.action == MLX_PRESS)
		fdf->settings->shift_y += 1;
	if (keydata.key == MLX_KEY_RIGHT && keydata.action == MLX_PRESS)
		fdf->settings->shift_x += 1;
	if (keydata.key == MLX_KEY_LEFT && keydata.action == MLX_PRESS)
		fdf->settings->shift_x -= 1;
	if (keydata.key == MLX_KEY_J && keydata.action == MLX_PRESS)
		puts("Hello ");
	if (keydata.key == MLX_KEY_K && keydata.action == MLX_RELEASE)
		puts("World");
	if (keydata.key == MLX_KEY_L && keydata.action == MLX_REPEAT)
		puts("!");
	draw(fdf->map, fdf);
}

void init_struct(t_fdf *fdf, char **av)
{
	int i;

	i = 0;
	fdf->width = 0;
	fdf->height = 0;
	fdf->error = 0;
	fdf->settings = malloc(sizeof(t_transform));
	fdf->settings->scale = 30;
	fdf->settings->shift_x = 0;
	fdf->settings->shift_y = 0;
	get_height(fdf, av[1]);
	fdf->map = calloc(sizeof(t_point *), fdf->height);
	if (!fdf->map)
		exit(92);
	i = -1;
	while (++i < fdf->height)
		fdf->map[i] = calloc(sizeof(t_point), fdf->width);
	fill_map(fdf, av[1]);
	fdf->mlx = mlx_init(1000, 1000, "FdF", true);
	if (!fdf->mlx)
		exit(1);
	fdf->img = mlx_new_image(fdf->mlx, 1920, 1080);
	mlx_image_to_window(fdf->mlx, fdf->img, 0 ,0);
	mlx_key_hook(fdf->mlx, &my_keyhook, fdf);
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
	mlx_loop(fdf->mlx);
	mlx_delete_image(fdf->mlx, fdf->img);
	mlx_terminate(fdf->mlx);
	return (0);
}

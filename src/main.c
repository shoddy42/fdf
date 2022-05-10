/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 21:16:22 by wkonings      #+#    #+#                 */
/*   Updated: 2022/05/10 02:16:57 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void my_keyhook(mlx_key_data_t keydata, void* param)
{
	t_fdf *fdf;

	fdf = param;

	if (mlx_is_key_down(fdf->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(fdf->mlx);
	if (keydata.action == MLX_RELEASE)
		return ;
	if (keydata.key == MLX_KEY_UP || mlx_is_key_down(fdf->mlx, MLX_KEY_UP))
		fdf->settings->shift_y -= 1;
	if (keydata.key == MLX_KEY_DOWN || mlx_is_key_down(fdf->mlx, MLX_KEY_DOWN))
		fdf->settings->shift_y += 1;
	if (keydata.key == MLX_KEY_RIGHT || mlx_is_key_down(fdf->mlx, MLX_KEY_RIGHT))
		fdf->settings->shift_x += 1;
	if (keydata.key == MLX_KEY_LEFT || mlx_is_key_down(fdf->mlx, MLX_KEY_LEFT))
		fdf->settings->shift_x -= 1;


	if (mlx_is_key_down(fdf->mlx, MLX_KEY_W))
		fdf->settings->shift_y -= 50;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_S))
		fdf->settings->shift_y += 50;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_D))
		fdf->settings->shift_x += 50;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_A))
		fdf->settings->shift_x -= 50;

		
	if (keydata.key == MLX_KEY_1 && keydata.action == MLX_PRESS)
		fdf->settings->scale -= 1;
	if (keydata.key == MLX_KEY_2 && keydata.action == MLX_PRESS)
		fdf->settings->scale += 1;
	if (keydata.key == MLX_KEY_E && keydata.action == MLX_PRESS)
		fdf->angle += 0.1;
	if (keydata.key == MLX_KEY_Q && keydata.action == MLX_PRESS)
		fdf->angle -= 0.1;
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
	fdf->angle = 0.8;
	fdf->settings = malloc(sizeof(t_transform));
	fdf->settings->scale = 30;
	fdf->settings->shift_x = 150;
	fdf->settings->shift_y = 150;
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
	fdf->img = mlx_new_image(fdf->mlx, 1000, 1000);
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

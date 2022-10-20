/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 21:16:22 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/20 20:32:07 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

//TODO: make a proper freeing function that goes through and frees all that needs to be freed.
void	error(t_fdf *data, const char *message, int code)
{
	// printf("%s\n", message); // illegal function
	free(data);
	exit (code);
}

void	scrollhook(double xdelta, double ydelta, void *param)
{
	t_fdf	*data;

	data = param;
	if (ydelta > 0)
		data->z_scale -= 0.05;
	if (ydelta < 0)
		data->z_scale += 0.05;
}

void	my_keyhook(mlx_key_data_t keydata, void	*param)
{
	t_fdf	*fdf;
	double	magnitude;

	fdf = param;
	magnitude = 1;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_LEFT_SHIFT))
		magnitude = 10;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(fdf->mlx);
	if (keydata.action == MLX_RELEASE)
		return ;
	if (keydata.key == MLX_KEY_UP || mlx_is_key_down(fdf->mlx, MLX_KEY_UP))
		fdf->transpose[Y] -= 1;
	if (keydata.key == MLX_KEY_DOWN || mlx_is_key_down(fdf->mlx, MLX_KEY_DOWN))
		fdf->transpose[Y] += 1;
	if (keydata.key == MLX_KEY_RIGHT || mlx_is_key_down(fdf->mlx, MLX_KEY_RIGHT))
		fdf->transpose[X] += 1;
	if (keydata.key == MLX_KEY_LEFT || mlx_is_key_down(fdf->mlx, MLX_KEY_LEFT))
		fdf->transpose[X] -= 1;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_W))
		fdf->transpose[Y] -= 50;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_S))
		fdf->transpose[Y] += 50;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_D))
		fdf->transpose[X] += 50;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_A))
		fdf->transpose[X] -= 50;

	if (keydata.key == MLX_KEY_Z)
		fdf->z_scale += 0.5 * magnitude;
	if (keydata.key == MLX_KEY_X)
		fdf->z_scale -= 0.5 * magnitude;
	if (keydata.key == MLX_KEY_9)
		if (fdf->distance > 1)
			fdf->distance -= 1 * magnitude;
	if (keydata.key == MLX_KEY_0)
		fdf->distance += 1 * magnitude;
	if (keydata.key == MLX_KEY_1)
		fdf->settings->scale -= 1 * magnitude;
	if (keydata.key == MLX_KEY_2)
		fdf->settings->scale += 1 * magnitude;
	if (keydata.key == MLX_KEY_E)
		fdf->angle += 0.03;
	if (keydata.key == MLX_KEY_Q)
		fdf->angle -= 0.03;
	if (keydata.key == MLX_KEY_R)
		fdf->x_angle += 0.03;
	if (keydata.key == MLX_KEY_F)
		fdf->y_angle += 0.03;
	if (keydata.key == MLX_KEY_V)
		fdf->z_angle += 0.03;
	if (keydata.key == MLX_KEY_T)
		fdf->x_angle -= 0.03;
	if (keydata.key == MLX_KEY_G)
		fdf->y_angle -= 0.03;
	if (keydata.key == MLX_KEY_B)
		fdf->z_angle -= 0.03;
	if (keydata.key == MLX_KEY_H)
		fdf->triangles *= -1;
	if (keydata.key == MLX_KEY_J)
	{
		fdf->perspective *= -1;
		fdf->settings->scale += 1200 * fdf->perspective;
	}
	if (keydata.key == MLX_KEY_C)
		fdf->colour *= -1;
	if (keydata.key == MLX_KEY_K)
		fdf->sub_z *= -1;
	if (fdf->settings->scale < 1)
		fdf->settings->scale = 1;
}

void	hook(void *param)
{
	t_fdf	*data;

	data = param;
	draw_vec(data->map, data);
}

void	resize(int32_t width, int32_t height, void *param)
{
	t_fdf	*data;
	int32_t	old_x;
	int32_t	old_y;

	data = param;
	ft_bzero(data->img->pixels, data->img->width
		* data->img->height * sizeof(uint32_t));
	old_x = data->img->width;
	old_y = data->img->height;
	mlx_resize_image(data->img, width, height);
	data->transpose[X] += (width / 2) - (old_x / 2);
	data->transpose[Y] += (height / 2) - (old_y / 2);
}

void	init_struct(t_fdf *data, char **av)
{
	int	i;

	data->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT,
			ft_strjoin("FdF - ", av[1]), true);
	if (!data->mlx)
		exit(1);
	data->z_max = INT32_MIN;
	data->z_min = INT32_MAX;
	get_height(data, av[1]);
	i = 0;
	data->map = calloc(data->height, sizeof(t_vec_point *));
	if (!data->map)
		exit(97); 
	i = -1;
	//TODO: calloc protection.
	while (++i < data->height)
		data->map[i] = calloc(data->width, sizeof(t_vec_point));
	data->settings = calloc(1, sizeof(t_transform));
	if (!data->settings)
		exit(120);
	fill_map(data, av[1]);

	data->z0 = (WINDOW_WIDTH / 2) / tan((FOV / 2.0)) * M_PI / 180.0;
	data->distance = 50;

	data->z_scale = 0.15;
	data->settings->scale = 30;

	data->angle = 0;
	data->x_angle = 0.4;
	data->y_angle = -0.4;
	data->z_angle = 0.7;

	data->triangles = -1;
	data->perspective = -1;
	data->colour = 1;
	data->sub_z = -1;
	
	data->transpose[X] = WINDOW_WIDTH / 2;
	data->transpose[Y] = WINDOW_HEIGHT / 2;

	data->img = mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	mlx_image_to_window(data->mlx, data->img, 0 ,0);
	// mlx_mouse_hook(data->mlx, &my_mousehook, data);
	mlx_scroll_hook(data->mlx, &scrollhook, data);
	mlx_key_hook(data->mlx, &my_keyhook, data);
	mlx_resize_hook(data->mlx, &resize, data);
	mlx_loop_hook(data->mlx, &hook, data);
	draw_vec(data->map, data);
}

int	main(int ac, char **av)
{
	t_fdf	data;

	ft_bzero(&data, sizeof(t_fdf));
	if (ac < 2)
	{
		printf ("bad input\n");
		return (12);
	}
	if (ac > 3)
	{
		printf ("too much input. only using one or smth\n");
	}
	init_struct(&data, av);
	draw_vec(data.map, &data);
	mlx_loop(data.mlx);
	mlx_delete_image(data.mlx, data.img);
	mlx_terminate(data.mlx);
	return (0);
}

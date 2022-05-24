/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 21:16:22 by wkonings      #+#    #+#                 */
/*   Updated: 2022/05/24 23:30:22 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

t_point	**scuffed_vec(void)
{
	t_point **cube;
	int x = -1;

	cube = calloc(8, sizeof(t_point *));
	while (++x < 8)
	{
		cube[x] = calloc(1, sizeof(t_point));
	}

	cube[0][0].x = 1;
	cube[0][0].y = 1;
	cube[0][0].z = 1;
	
	cube[1][0].x = 1;
	cube[1][0].y = 5;
	cube[1][0].z = 1;

	cube[2][0].x = 5;
	cube[2][0].y = 5;
	cube[2][0].z = 1;

	cube[3][0].x = 5;
	cube[3][0].y = 1;
	cube[3][0].z = 1;

	cube[4][0].x = 1;
	cube[4][0].y = 1;
	cube[4][0].z = 5;
	
	cube[5][0].x = 1;
	cube[5][0].y = 5;
	cube[5][0].z = 5;

	cube[6][0].x = 5;
	cube[6][0].y = 5;
	cube[6][0].z = 5;

	cube[7][0].x = 5;
	cube[7][0].y = 1;
	cube[7][0].z = 5;
	return (cube);
}

void	error(t_fdf *data, const char *message, int code)
{
	printf("%s\n", message);
	//TODO: make a proper freeing function that goes through and frees all that needs to be freed.
	free(data);
	exit (code);
}

void	scrollhook(double xdelta, double ydelta, void *param)
{
	t_fdf *data;

	data = param;
	if (ydelta > 0)
		data->z_scale -= 0.05;
	if (ydelta < 0)
		data->z_scale += 0.05;
	draw(data->map, data);
}

void	my_keyhook(mlx_key_data_t keydata, void	*param)
{
	t_fdf *fdf;
	double magnitude;

	fdf = param;

	magnitude = 1;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_LEFT_SHIFT))
		magnitude = 10;
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
	if (keydata.key == MLX_KEY_K)
	{
		fdf->sub_z *= -1;
		if (fdf->sub_z == true)
			printf("sub z on\n");
		else
			printf("sub z off\n");
	}

// 	if (keydata.key == MLX_KEY_L && keydata.action == MLX_REPEAT)
// 		puts("!");
	if (fdf->settings->scale < 1)
		fdf->settings->scale = 1;

	// printf("dist: %f\n", fdf->distance);
	draw(fdf->map, fdf);

	// draw_cube(fdf);
}

void	resize(int32_t width, int32_t height, void *param)
{
	t_fdf *data;
	int old_x;
	int old_y;

	data = param;
	ft_bzero(data->img->pixels, data->img->width * data->img->height * sizeof(uint32_t));
	old_x = data->img->width;
	old_y = data->img->height;
	mlx_resize_image(data->img, width, height);
	data->settings->shift_x += (width / 2) - (old_x / 2);
	data->settings->shift_y += (height / 2) - (old_y / 2);
	draw(data->map, data);
}

t_point	translate(t_point original, t_point translation, t_fdf *data)
{
	t_point ret;
	ret.x = original.x + translation.x;
	ret.y = original.y + translation.y;
	ret.z = original.z + translation.z;
	return (ret);
}

void rotate(t_point *original, t_point rotation, t_fdf *data)
{
	t_point ret;
	ret.x = original->x * (cos(rotation.z) * cos(rotation.y)) + 
		original->y * (cos(rotation.z) * sin(rotation.y) * sin(rotation.x) - sin(rotation.z) * cos(rotation.x)) +
		original->z * (cos(rotation.z) * sin(rotation.y) * cos(rotation.x) + sin(rotation.z) * sin(rotation.x));
	ret.y = original->x * (sin(rotation.z) * cos(rotation.y)) +
		original->y * (sin(rotation.z) * sin(rotation.y) * sin(rotation.x) + cos(rotation.z) * cos(rotation.x)) +
		original->z * (sin(rotation.z) * sin(rotation.y) * cos(rotation.x) - cos(rotation.z) * sin(rotation.x));
	ret.z = original->x * (- sin(rotation.y)) +
		original->y * (cos(rotation.y) * sin(rotation.x)) +
		original->z * (cos(rotation.y) * cos(rotation.x));
	*original = ret;
}

void apply_perspective(t_point *original, t_fdf *data)
{
	t_point ret;
	ret.x = original->x * data->z0 / (data->z0 + original->z);
	ret.x = original->y * data->z0 / (data->z0 + original->z);
	ret.z = original->z;
	*original = ret;
}

void	init_struct(t_fdf *data, char **av)
{
	int i;

	data->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "FdF", true);
	if (!data->mlx)
		exit(1);
	data->z_max = INT32_MIN;
	data->z_min = INT32_MAX;
	get_height(data, av[1]);
	i = 0;
	data->map = calloc(data->height, sizeof(t_point *));
	if (!data->map)
		exit(92);
	i = -1;
	while (++i < data->height)
		data->map[i] = calloc(data->width, sizeof(t_point));
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
	data->sub_z = -1;
	data->settings->shift_x = WINDOW_WIDTH / 2;
	data->settings->shift_y = WINDOW_HEIGHT / 2;
	data->img = mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	mlx_image_to_window(data->mlx, data->img, 0 ,0);
	// mlx_mouse_hook(data->mlx, &my_mousehook, data);
	mlx_scroll_hook(data->mlx, &scrollhook, data);
	mlx_key_hook(data->mlx, &my_keyhook, data);
	mlx_resize_hook(data->mlx, &resize, data);
	draw(data->map, data);
}


int	main(int ac, char **av)
{
	unsigned int	 i = 0;
	unsigned int j = 0;
	t_fdf	data;

	ft_bzero(&data, sizeof(t_fdf));
	if (ac < 2)
	{
		printf ("bad input\n");
		return (12);
	}
	init_struct(&data, av);
	// draw_cube(&data);
	draw(data.map, &data);
	mlx_loop(data.mlx);
	mlx_delete_image(data.mlx, data.img);
	mlx_terminate(data.mlx);
	return (0);
}

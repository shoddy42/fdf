/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 21:16:22 by wkonings      #+#    #+#                 */
/*   Updated: 2022/05/30 09:23:57 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

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

	// only adjusts z_scale -- should be finished
	data = param;
	if (ydelta > 0)
		data->z_scale -= 0.05;
	if (ydelta < 0)
		data->z_scale += 0.05;
	draw(data->map, data);
}

void	my_keyhook(mlx_key_data_t keydata, void	*param)
{

	// needs refactoring entirely, probably a subfunction maybe even two.
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
	if (keydata.key == MLX_KEY_L)
	{
		fdf->vector_based *= -1;
		if (fdf->vector_based == true)
			printf("vector on\n");
		else
			printf("vector off\n");
	}
	if (keydata.key == MLX_KEY_J)
	{
		fdf->perspective *= -1;
		fdf->settings->scale += 1200 * fdf->perspective;
	}
	if (keydata.key == MLX_KEY_C)
		fdf->colour *= -1;
	if (keydata.key == MLX_KEY_K)
	{
		fdf->sub_z *= -1;
		if (fdf->sub_z == true)
			printf("sub z on\n");
		else
			printf("sub z off\n");
	}
	if (fdf->settings->scale < 1)
		fdf->settings->scale = 1;
	// if (mlx_get_time() != fdf->last_sec)
	// {
	// 	fdf->last_sec = mlx_get_time();
	// 	printf("FPS: %i\n", fdf->frames);
	// 	fdf->frames = 0;
	// }
	if (fdf->vector_based == true)
		draw_vec(fdf->map2, fdf);
	else
		draw(fdf->map, fdf);
}

void	resize(int32_t width, int32_t height, void *param)
{
	// fixes resizing issues -- should be finished
	t_fdf *data;
	int32_t old_x;
	int32_t old_y;

	data = param;
	ft_bzero(data->img->pixels, data->img->width * data->img->height * sizeof(uint32_t));
	old_x = data->img->width;
	old_y = data->img->height;
	mlx_resize_image(data->img, width, height);
	data->settings->shift_x += (width / 2) - (old_x / 2);
	data->settings->shift_y += (height / 2) - (old_y / 2);
	draw(data->map, data);
	// draw_vec(data->map2, &data);

}

t_point	translate(t_point original, t_point translation)
{
	t_point ret;
	// currently not in use, should replace shift_x and shift_y in a single variable.
	ret.x = original.x + translation.x;
	ret.y = original.y + translation.y;
	ret.z = original.z + translation.z;
	return (ret);
}

// void rotate(t_point *original, t_point rotation)
// {
// 	// currently not in use, should replace all 3 individual rotations at the same time.
// 	t_point ret;
// 	ret.x = original->x * (cos(rotation.z) * cos(rotation.y)) + 
// 		original->y * (cos(rotation.z) * sin(rotation.y) * sin(rotation.x) - sin(rotation.z) * cos(rotation.x)) +
// 		original->z * (cos(rotation.z) * sin(rotation.y) * cos(rotation.x) + sin(rotation.z) * sin(rotation.x));
// 	ret.y = original->x * (sin(rotation.z) * cos(rotation.y)) +
// 		original->y * (sin(rotation.z) * sin(rotation.y) * sin(rotation.x) + cos(rotation.z) * cos(rotation.x)) +
// 		original->z * (sin(rotation.z) * sin(rotation.y) * cos(rotation.x) - cos(rotation.z) * sin(rotation.x));
// 	ret.z = original->x * (- sin(rotation.y)) +
// 		original->y * (cos(rotation.y) * sin(rotation.x)) +
// 		original->z * (cos(rotation.y) * cos(rotation.x));
// 	*original = ret;
// }

// void apply_perspective(t_point *original, t_fdf *data)
// {
// 	// currently broke.
// 	t_point ret;
// 	ret.x = original->x * data->z0 / (data->z0 + original->z);
// 	ret.x = original->y * data->z0 / (data->z0 + original->z);
// 	ret.z = original->z;
// 	*original = ret;
// }

void	init_struct(t_fdf *data, char **av)
{
	int i;
	// need to subdivide this into init_struct and init_map, should call init_map in main separately.
	// might also make a sub function for init_mlx.
	data->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, ft_strjoin("FdF - ", av[1]), true);
	if (!data->mlx)
		exit(1);
	data->z_max = INT32_MIN;
	data->z_min = INT32_MAX;
	get_height(data, av[1]);
	i = 0;
	data->map = calloc(data->height, sizeof(t_point *));
	data->map2 = calloc(data->height, sizeof(t_vec_point *));
	if (!data->map2)
		exit(97); 
	if (!data->map)
		exit(92);
	i = -1;
	while (++i < data->height)
		data->map[i] = calloc(data->width, sizeof(t_point));
	i = -1;
	while (++i < data->height)
		data->map2[i] = calloc(data->width, sizeof(t_vec_point));
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
	data->colour = -1;
	data->sub_z = -1;
	data->vector_based = -1;
	data->settings->shift_x = WINDOW_WIDTH / 2;
	data->settings->shift_y = WINDOW_HEIGHT / 2;
	data->img = mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	mlx_image_to_window(data->mlx, data->img, 0 ,0);
	// mlx_mouse_hook(data->mlx, &my_mousehook, data);
	mlx_scroll_hook(data->mlx, &scrollhook, data);
	mlx_key_hook(data->mlx, &my_keyhook, data);
	mlx_resize_hook(data->mlx, &resize, data);
	draw(data->map2, data);
}


int	main(int ac, char **av)
{
	t_fdf	data;

	ft_bzero(&data, sizeof(t_fdf));
	// if ac < 2, give warning and draw cube
	if (ac < 2)
	{
		printf ("bad input\n");
		return (12);
	}
	// if ac > 3 give warning there were too many args, still attempt to move on
	init_struct(&data, av);
	// draw(data.map, &data);
	draw_vec(data.map2, &data);
	mlx_loop(data.mlx);
	mlx_delete_image(data.mlx, data.img);
	mlx_terminate(data.mlx);
	return (0);
}

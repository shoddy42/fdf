/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 21:16:22 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/20 23:32:58 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	set_default_values(t_fdf *data)
{
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
}

//TODO: calloc protection.
void	fdf_init_alloc(t_fdf *data, char **av)
{
	int	i;

	i = 0;
	get_height(data, av[1]);
	data->map = calloc(data->height, sizeof(t_vec_point *));
	if (!data->map)
		fdf_error(data, "map allocation failed.\n", -1);
	i = -1;
	while (++i < data->height)
	{
		data->map[i] = calloc(data->width, sizeof(t_vec_point));
		if (!data->map[i])
			fdf_error(data, "map[%i] allocation failed.\n", -1);
	}
	data->settings = calloc(1, sizeof(t_transform));
	if (!data->settings)
		fdf_error(data, "settings allocation failed\n", -1);
}

void	init_struct(t_fdf *data, char **av)
{
	data->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT,
			ft_strjoin("FdF - ", av[1]), true);
	if (!data->mlx)
		exit(1);
	data->z_max = INT32_MIN;
	data->z_min = INT32_MAX;
	fdf_init_alloc(data, av);
	fill_map(data, av[1]);
	set_default_values(data);
	data->img = mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	mlx_image_to_window(data->mlx, data->img, 0, 0);
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

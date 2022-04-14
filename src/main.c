/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 21:16:22 by wkonings      #+#    #+#                 */
/*   Updated: 2022/04/14 17:42:38 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// void	square(t_fdf *fdf, int color)
// {
// 	int	x;
// 	int	y;

// 	x = 500;
// 	y = 500;
// 	while (y <= 1000)
// 	{
// 		mlx_put_pixel(fdf->img, x, y, color);
// 		x = 500;
// 		while (x <= 1000)
// 		{
// 			mlx_put_pixel(fdf->img, x, y, color);
// 			x++;
// 		}
// 		y++;
// 	}
// }

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

int get_width(char *str)
{
	int ret;
	int i;

	i = -1;
	ret = 0;
	while (str[++i])
	{
		while (str[i] == ' ')
			i++;
		if (str[i] >= '0' && str[i] <= '9')
		{
			ret++;
			printf("test [%s] (%i)\n", str, ret);
			while (str[i] >= '0' && str[i] <= '9')
				i++;
		}
		// i++;
	}
	printf("Ret: %i\n", ret);
	return (ret);
}

int get_height(t_fdf *fdf, char *name)
{
	int fd;
	int i;
	int width;
	char *line;
	int read;

	i = -1;
	read = 42;
 	fd = open(name, O_RDONLY);
	if (!fd)
		exit (4);
	while (read > 0)
	{
		read = get_next_line(fd, &line);
		printf("input line: [%s]\n", line);
		while (line[i] < '0' || line[i] > '9' && line[i] != '\0')
			i++;
		
		if (line[i] >= '0' && line[i] <= '9')
		{
			fdf->height++;
			// if (get_width(line) > 0);
			// 	fdf->width = get_width(line);
		}
		free(line);
	}
	printf("new H: [%i] W:[%i]\n", fdf->height, fdf->width);
	return (0);
}

int	readfile(t_fdf *fdf, char *name)
{
	int fd;
	int i;
	char *line;
	int read;

	i = 0;
	read = 42;
 	fd = open(name, O_RDONLY);
	if (!fd)
		exit (124);
	while (read > 0)
	{
		read = get_next_line(fd, &line);
		printf("input line: [%s]\n", line);
		if (fdf->height == 0)
		{
			while(line[i + fdf->width])
			{
				if (line[i + fdf->width] == ' ')
					i++;
				else
					fdf->width++;
			}
		}
		fdf->height++;
		free(line);
	}
	close(fd);
	printf("height: [%i] width: [%i]\n", fdf->height, fdf->width);
	return (0);
}

int fill_map(t_fdf *fdf, char *name)
{
	int fd;
	int x;
	int y;
	int read;
	char *line;

	x = -1;
	y = 0;
	fd = open(name, O_RDONLY);
	read = 42;
	while(read > 0)
	{
		read = get_next_line(fd, &line);
		printf("line???: %s\n", line);
		x = -1;
		while(++x < fdf->width)
		{
			while(*line == ' ' || *line == '\n')
				line++;
			fdf->map[y][x] = ft_atoi(line);
			while(*line >= '0' && *line <= '9')
				line++;
		}
		y++;
	}
	x = 0;
	y = -1;
	while(++y < fdf->height)
	{
		x = 0;
		printf("e[%i]", fdf->map[y][0]);
		while(++x < fdf->width)
			printf("[%i]", fdf->map[y][x]);
		printf("\n");
	}
	return (0);
}

void init_struct(t_fdf *fdf, char **av)
{
	int i;

	i = 0;
	fdf->width = 0;
	fdf->height = 0;
	fdf->error = 0;
	get_height(fdf, av[1]);
	// readfile(fdf, av[1]);
	// fdf->map = calloc(sizeof(int *), fdf->height);
	// if (!fdf->map)
	// 	exit(92);
	// i = -1;
	// while (++i < fdf->height)
	// 	fdf->map[i] = calloc(sizeof(int), fdf->width);
	// fill_map(fdf, av[1]);
	fdf->mlx = mlx_init(1920, 1080, "FdF", true);
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
	// square(fdf, 0xFF00FF);
	mlx_loop(fdf->mlx);
	mlx_delete_image(fdf->mlx, fdf->img);
	mlx_terminate(fdf->mlx);
	return (0);
}

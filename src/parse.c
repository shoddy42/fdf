/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/02 22:05:25 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/20 23:06:44 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

// this is garbage. want to make it gradient.
//colours should be applied only once, preferably during creation.
// uint32_t	get_col(t_point a, t_fdf *data)
uint32_t	get_col(t_vec_point point, t_fdf *data)
{
	t_gradient		col;

	col.water = 0x0000FFFF;
	col.sand = 0xFFFF00FF;
	col.grass = 0x00FF00FF;
	col.rock = 0x555555FF;
	col.snow = 0xFFFFFFFF;
	if ((int)point.vec[Z] >= data->z_max)
		return (col.snow);
	if ((int)point.vec[Z] >= data->z_max * 0.75)
		return (col.rock);
	if ((int)point.vec[Z] >= data->z_max * 0.5)
		return (col.grass);
	if ((int)point.vec[Z] >= data->z_max * 0.25)
		return (col.sand);
	return (col.water);
}

void	assign_col(t_fdf *data)
{
	int	x;
	int	y;

	y = -1;
	while (++y < data->height)
	{
		x = -1;
		while (++x < data->width)
			data->map[y][x].colour = get_col(data->map[y][x], data);
	}
}

int	get_map(t_fdf *fdf, int fd, int y, int x)
{
	char	*line;
	int		read;

	read = 42;
	while (read > 0 && y < fdf->height)
	{
		read = get_next_line(fd, &line);
		x = -1;
		while (++x < fdf->width)
		{
			while (*line == ' ' || *line == '\n')
				line++;
			fdf->map[y][x].vec[Z] = ft_atoi(line);
			if (fdf->map[y][x].vec[Z] > fdf->z_max)
				fdf->z_max = fdf->map[y][x].vec[Z];
			if (fdf->map[y][x].vec[Z] < fdf->z_min)
				fdf->z_min = fdf->map[y][x].vec[Z];
			fdf->map[y][x].vec[Y] = y;
			fdf->map[y][x].vec[X] = x;
			while (*line >= '0' && *line <= '9')
				line++;
		}
		y++;
	}
	return (0);
}

//todo: ACTUAL ERROR
int	fill_map(t_fdf *fdf, char *name)
{
	int		fd;

	fd = open(name, O_RDONLY);
	if (fd < 0)
	{
		printf("open failed\n");
		exit(212);
	}
	get_map(fdf, fd, 0, -1);
	assign_col(fdf);
	return (0);
}

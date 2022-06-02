/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/02 22:05:25 by wkonings      #+#    #+#                 */
/*   Updated: 2022/05/31 14:18:11 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

int	readfile(t_fdf *fdf, char *name)
{
	int		fd;
	int		i;
	int		read;
	char	*line;

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
			while (line[i + fdf->width])
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

uint32_t	get_col(t_point a, t_fdf *data)
{
	// this is garbage. want to make it gradient. colours should be applied only once, preferably during creation.
	t_gradient	col;
	t_point		tmp;

	col.water = 0x0000FFFF;
	col.sand  =	0xFFFF00FF;
	col.grass =	0x00FF00FF;
	col.rock  = 0x555555FF;
	col.snow  = 0xFFFFFFFF;

	tmp = a;
	if ((int)tmp.z >= data->z_max)
		return (col.snow);
	if ((int)tmp.z >= data->z_max * 0.75)
		return (col.rock);
	if ((int)tmp.z >= data->z_max * 0.5)
		return (col.grass);
	if ((int)tmp.z >= data->z_max * 0.25)
		return (col.sand);
	return (col.water);
}

void	assign_col(t_fdf *data)
{
	int x;
	int y;

	y = 0;
	while (y < data->height)
	{
		x = 0;
		while (x < data->width)
		{
			data->map[y][x].colour = get_col(data->map[y][x], data);
			data->map2[y][x].colour = get_col(data->map[y][x], data);
			x++;
		}
		y++;
	}
}

int	fill_map(t_fdf *fdf, char *name)
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
	while(read > 0 && y < fdf->height)
	{
		read = get_next_line(fd, &line);
		printf("line???: %s\n", line);
		x = -1;
		while(++x < fdf->width)
		{
			while(*line == ' ' || *line == '\n')
				line++;
			fdf->map[y][x].z = ft_atoi(line);
			fdf->map2[y][x].vec[Z] = ft_atoi(line);
			if (fdf->map[y][x].z > fdf->z_max)
				fdf->z_max = fdf->map[y][x].z;
			if (fdf->map[y][x].z < fdf->z_min)
				fdf->z_min = fdf->map[y][x].z;
			fdf->map[y][x].y = y;
			fdf->map2[y][x].vec[Y] = y;
			fdf->map[y][x].x = x;
			fdf->map2[y][x].vec[X] = x;
			while(*line >= '0' && *line <= '9')
				line++;
		}
		y++;
	}
	assign_col(fdf);
	printf("zmin %i zmax %i\n", fdf->z_min, fdf->z_max);
	x = 0;
	y = -1;
	printf("final array H: [%i] W: [%i]\n", fdf->height, fdf->width);
	while(++y < fdf->height)
	{
		x = -1;
		// printf("[%i]", fdf->map[y][0]);
		while(++x < fdf->width)
		{
			printf("[%2i]", (int)fdf->map2[y][x].vec[Z]);
			// printf("[%f]", fdf->map[y][x].z);
		}
		printf("\n");
	}
	return (0);
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
			while (str[i] >= '0' && str[i] <= '9')
				i++;
		}
	}
	printf("setting width: %i\n", ret);
	return (ret);
}

int get_height(t_fdf *fdf, char *name)
{
	int fd;
	int i;
	int width;
	char *line;
	int read;

	read = 42;
 	fd = open(name, O_RDONLY);
	if (!fd)
		exit (4);
	while (read > 0)
	{
		read = get_next_line(fd, &line);
		i = 0;
		while ((line[i] < '0' || line[i] > '9') && line[i])
			i++;
		if (line[i] >= '0' && line[i] <= '9')
		{
			fdf->height++;
			if (fdf->width == 0)
				fdf->width = get_width(line);
		}
		free(line);
	}
	printf("new H: [%i] W:[%i]\n", fdf->height, fdf->width);
	return (0);
}
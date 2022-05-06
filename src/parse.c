/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/02 22:05:25 by wkonings      #+#    #+#                 */
/*   Updated: 2022/05/07 00:24:56 by wkonings      ########   odam.nl         */
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
			// fdf->map[y][x] = ft_atoi(line);
			fdf->map[y][x].z = ft_atoi(line);
			fdf->map[y][x].y = y;
			fdf->map[y][x].x = x;
			printf("map[%i][%i]: x = %f, y = %f, z = %f\n", y, x, fdf->map[y][x].x, fdf->map[y][x].y, fdf->map[y][x].z);
			while(*line >= '0' && *line <= '9')
				line++;
		}
		y++;
	}
	x = 0;
	y = -1;
	printf("final array H: [%i] W: [%i]\n", fdf->height, fdf->width);
	while(++y < fdf->height)
	{
		x = -1;
		// printf("[%i]", fdf->map[y][0]);
		while(++x < fdf->width)
			printf("[%f]", fdf->map[y][x].z);
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
		// printf("i: %i str: %s\n", i, line);
		if (line[i] >= '0' && line[i] <= '9')
		{
			// printf("EEE: [%s] with: [%c]\n", line, line[i]);
			fdf->height++;
			if (fdf->width == 0)
				fdf->width = get_width(line);
		}
		free(line);
	}
	printf("new H: [%i] W:[%i]\n", fdf->height, fdf->width);
	return (0);
}
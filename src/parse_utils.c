/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/20 21:29:27 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/20 22:56:04 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

int	get_width(char *str)
{
	int	ret;
	int	i;

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
	return (ret);
}

int	get_height(t_fdf *fdf, char *name)
{
	int		fd;
	int		i;
	int		width;
	char	*line;
	int		read;

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
	return (0);
}

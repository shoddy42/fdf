/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fdf.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 21:54:58 by wkonings      #+#    #+#                 */
/*   Updated: 2022/05/09 22:40:25 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H
# include "MLX42/include/MLX42/MLX42.h"
# include "src/get_next_line_bonus.h"
# include "libft/libft.h"
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <math.h>
// typedef struct	s_mlx
// {
// 	mlx_image_t *img;
// 	void	*window;
// 	void	*context;
// 	int		width;
// 	int		height;
// 	double	delta_time;
// }				t_mlx;

typedef struct s_line
{
	float	x_step;
	float	y_step;
	float	x;
	float	y;
}				t_line;

typedef struct s_point
{
	float		x;
	float		y;
	float		z;
	int			angle;
	uint32_t	colour;
}				t_point;

typedef struct s_transform
{
	float	max;
	float	step_x;
	float	step_y;
	float	shift_x;
	float	shift_y;
	int		scale;
}				t_transform;

typedef struct s_fdf
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_transform	*settings;
	t_point		**map;
	int			error;
	int			width;
	int			height;
	int			isometric;
	float			angle;
}				t_fdf;

void	draw(t_point **matrix, t_fdf *fdf);
// void	bresenham(float x, float y, float x1, float y1, t_fdf *fdf, uint32_t color);
void	bresenham_p(t_point a, t_point b, t_fdf *fdf);

/****************************\
      	Get Next Line
\****************************/

#endif

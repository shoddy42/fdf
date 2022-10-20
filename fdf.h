/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fdf.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 21:54:58 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/20 20:25:43 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H
# define WINDOW_WIDTH 1000
# define WINDOW_HEIGHT 1000
# define FOV 120
# define X 0
# define Y 1
# define Z 2
# define W 3
# include "MLX42/include/MLX42/MLX42.h"
# include "src/get_next_line_bonus.h"
# include "libft/libft.h"
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <math.h>

typedef double	t_vec __attribute__ ((vector_size (4 * sizeof(double))));

typedef struct s_interpolate
{
	unsigned char	r1;
	unsigned char	r2;
	unsigned char	g1;
	unsigned char	g2;
	unsigned char	b1;
	unsigned char	b2;
	unsigned char	a1;
	unsigned char	a2;
}				t_interpolate;

typedef struct s_point
{
	double		x;
	double		y;
	double		z;
	uint32_t	colour;
}				t_point;

typedef struct s_vec_point
{
	t_vec		vec;
	uint32_t	colour;
}	t_vec_point;

typedef struct s_transform
{
	float	max;
	float	step_x;
	float	step_y;
	int		scale;
	float	start_x;
	float	start_y;
	double	a_z;
	double	b_z;
	float	fraction;
	float	f_step;
	t_vec	shift;
}				t_transform;

typedef struct s_gradient
{
	uint32_t	water;
	uint32_t	sand;
	uint32_t	grass;
	uint32_t	rock;
	uint32_t	snow;
}				t_gradient;

typedef struct s_fdf
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	char		*title;
	// t_point		**map;
	t_vec_point	**map;

	t_transform	*settings;
	t_vec		transpose;
	int			width;
	int			height;
	double		angle;
	double		x_angle;
	double		y_angle;
	double		z_angle;
	double		distance;
	double		z_scale;
	double		z0;
	int			perspective;
	int			triangles;
	int			sub_z;
	int			z_max;
	int			z_min;
	int			colour;
	int			frames;
	int			last_sec;
}				t_fdf;

// void	draw(t_point **matrix, t_fdf *fdf);
void	draw_vec(t_vec_point **matrix, t_fdf *fdf);
// void	bresenham_p(t_point a, t_point b, t_fdf *fdf);
void	rotate(t_vec_point *original, t_fdf *data);

/****************************\
 * 		util functions		*
\****************************/

// t_point	**scuffed_vec(void);
// void	apply_perspective(t_point *original, t_fdf *data);

// 0x88428C0 == nice greeny
// 0x9E4C8C0 == spectraly blue
// 0xB2E7E0 == sky blue
#endif

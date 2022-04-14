/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fdf.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 21:54:58 by wkonings      #+#    #+#                 */
/*   Updated: 2022/04/13 20:32:19 by wkonings      ########   odam.nl         */
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

// typedef struct	s_mlx
// {
// 	mlx_image_t *img;
// 	void	*window;
// 	void	*context;
// 	int		width;
// 	int		height;
// 	double	delta_time;
// }				t_mlx;

typedef struct	s_fdf
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	int			error;
	int			width;
	int			height;
	int			**map;
}				t_fdf;

/****************************\
      	Get Next Line
\****************************/

#endif
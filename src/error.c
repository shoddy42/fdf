/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/20 21:02:13 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/20 23:34:57 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	fdf_error(t_fdf	*data, const char *message, int code)
{
	perror(message);
	mlx_delete_image(data->mlx, data->img);
	mlx_terminate(data->mlx);
	exit(code);
}

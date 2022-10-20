/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   keyhooks.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/20 20:59:27 by wkonings      #+#    #+#                 */
/*   Updated: 2022/10/20 22:53:56 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	transpose(mlx_key_data_t	keydata, t_fdf *fdf, double magnitude)
{
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_UP))
		fdf->transpose[Y] -= 1 * magnitude;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_DOWN))
		fdf->transpose[Y] += 1 * magnitude;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_RIGHT))
		fdf->transpose[X] += 1 * magnitude;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_LEFT))
		fdf->transpose[X] -= 1 * magnitude;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_W))
		fdf->transpose[Y] -= 10 * magnitude;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_S))
		fdf->transpose[Y] += 10 * magnitude;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_D))
		fdf->transpose[X] += 10 * magnitude;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_A))
		fdf->transpose[X] -= 10 * magnitude;
}

void	scale(mlx_key_data_t keydata, t_fdf *fdf, double magnitude)
{
	if (keydata.key == MLX_KEY_Z)
		fdf->z_scale += 0.5 * magnitude;
	if (keydata.key == MLX_KEY_X)
		fdf->z_scale -= 0.5 * magnitude;
	if (keydata.key == MLX_KEY_9)
		if (fdf->distance > 1)
			fdf->distance -= 1 * magnitude;
	if (keydata.key == MLX_KEY_0)
		fdf->distance += 1 * magnitude;
	if (keydata.key == MLX_KEY_1)
		fdf->settings->scale -= 1 * magnitude;
	if (keydata.key == MLX_KEY_2)
		fdf->settings->scale += 1 * magnitude;
}

void	angle(mlx_key_data_t keydata, t_fdf *fdf, double magnitude)
{
	if (magnitude > 1)
		magnitude = 2;
	if (keydata.key == MLX_KEY_E)
		fdf->angle += 0.03 * magnitude;
	if (keydata.key == MLX_KEY_Q)
		fdf->angle -= 0.03 * magnitude;
	if (keydata.key == MLX_KEY_R)
		fdf->x_angle += 0.03 * magnitude;
	if (keydata.key == MLX_KEY_F)
		fdf->y_angle += 0.03 * magnitude;
	if (keydata.key == MLX_KEY_V)
		fdf->z_angle += 0.03 * magnitude;
	if (keydata.key == MLX_KEY_T)
		fdf->x_angle -= 0.03 * magnitude;
	if (keydata.key == MLX_KEY_G)
		fdf->y_angle -= 0.03 * magnitude;
	if (keydata.key == MLX_KEY_B)
		fdf->z_angle -= 0.03 * magnitude;
}

void	setting_toggles(mlx_key_data_t keydata, t_fdf *fdf)
{
	if (keydata.key == MLX_KEY_H)
		fdf->triangles *= -1;
	if (keydata.key == MLX_KEY_C)
		fdf->colour *= -1;
	if (keydata.key == MLX_KEY_K)
		fdf->sub_z *= -1;
	if (keydata.key == MLX_KEY_J)
	{
		fdf->perspective *= -1;
		fdf->settings->scale += 1200 * fdf->perspective;
	}
}

void	my_keyhook(mlx_key_data_t keydata, void	*param)
{
	t_fdf	*fdf;
	double	magnitude;

	fdf = param;
	magnitude = 1;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(fdf->mlx);
	if (keydata.action == MLX_RELEASE)
		return ;
	if (mlx_is_key_down(fdf->mlx, MLX_KEY_LEFT_SHIFT))
		magnitude *= 10;
	transpose(keydata, fdf, magnitude);
	scale(keydata, fdf, magnitude);
	angle(keydata, fdf, magnitude);
	setting_toggles(keydata, fdf);
	if (fdf->settings->scale < 1)
		fdf->settings->scale = 1;
}

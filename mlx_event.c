/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_event.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 02:05:08 by tbousque          #+#    #+#             */
/*   Updated: 2022/06/25 20:25:54 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_event.h"

static t_vec3d	key_move(int keycode, t_mlx_info *info)
{
	t_vec3d	dir;
	t_vec3d	cam_dir;

	dir = (t_vec3d){0, 0, 0};
	if (keycode == KEY_D)
		dir.x += MOVE_SPEED;
	if (keycode == KEY_A)
		dir.x -= MOVE_SPEED;
	if (keycode == KEY_W)
		dir.z += MOVE_SPEED;
	if (keycode == KEY_S)
		dir.z -= MOVE_SPEED;
	if (keycode == KEY_SPACE)
		dir.y -= MOVE_SPEED;
	if (keycode == KEY_C)
		dir.y += MOVE_SPEED;
	cam_dir = vec3d_projected(dir, mat4x4_rotate_y(info->camera.yaw));
	info->camera.pos = vec3d_add((&info->camera.pos), &cam_dir);
	return (dir);
}

static void	isometric_movement(t_mesh *mesh, t_vec3d	dir)
{
	size_t			i;
	t_vec3d			v;
	static float	scale = 0;
	static t_vec3d	all_dir = {0, 10, 0};

	all_dir = vec3d_add(&all_dir, &dir);
	scale = 1 / all_dir.y * 0.5;
	i = 0;
	while (i < mesh->vertices_size)
	{
		v = mesh->vertices_projected[i];
		v.z = 0;
		v.x -= all_dir.x * ISO_MOVE_SPEED;
		v.y += all_dir.z * ISO_MOVE_SPEED;
		if (scale != 0)
		{
			v.x *= scale;
			v.y *= scale;
		}
		mesh->vertices_projected[i] = v;
		i++;
	}
}

void	do_proj(enum e_proj_mode proj_mode, t_mlx_info *info, t_vec3d key_dir, \
		int key_code)
{
	static float	scale = 0.1;
	t_mat4x4		iso_proj;

	if (key_code == KEY_PLUS)
		scale += SCALE_FDF;
	else if (key_code == KEY_MINUS)
		scale -= SCALE_FDF;
	if (proj_mode == mode_perspective)
	{
		mesh_project(info->map, info->img, \
		mat4x4_product(\
			get_view_mat(&(info->camera)), \
			get_projection_matrix(info->proj)));
	}
	else
	{	
		iso_proj = get_projection_matrix(info->proj);
		iso_proj = mat4x4_product(mat4x4_scale(1, 1, scale), iso_proj);
		mesh_project(info->map, info->img, iso_proj);
		isometric_movement(info->map, key_dir);
	}
}

//printf("%d\n", keycode);
int	key_event(int keycode, t_mlx_info *info)
{
	t_vec3d	key_dir;

	if (keycode == KEY_P)
		switch_projection_mode(&(info->proj));
	key_dir = key_move(keycode, info);
	if (keycode == 114)
		image_clear(info->img);
	if (keycode == 65307 || keycode == 65477)
	{
		mlx_context_free(info);
		exit(0);
	}
	if (keycode == KEY_L)
		load_new_file(info);
	image_clear(info->img);
	do_proj(info->proj.mode, info, key_dir, keycode);
	mesh_draw(info->map, info->img);
	mlx_put_image_to_window(info->mlx_ptr, info->win_ptr, info->img.ptr, 0, 0);
	return (0);
}

int	mouse_event(int x, int y, t_mlx_info *info)
{
	static t_vec2di	last = {.x = 0, .y = 0};
	t_vec2di		current;
	t_vec2di		relative;

	current = vec2di(x, y);
	relative = vec2di_sub(last, vec2di(x, y));
	last = current;
	info->camera.yaw += (relative.x / 100.0f);
	image_clear(info->img);
	if (info->proj.mode == mode_perspective)
	{
		mesh_project(info->map, info->img, \
			mat4x4_product(\
				get_view_mat(&(info->camera)), \
				get_projection_matrix(info->proj)));
		mesh_draw(info->map, info->img);
		mlx_put_image_to_window(info->mlx_ptr, info->win_ptr, \
			info->img.ptr, 0, 0);
	}
	return (0);
}

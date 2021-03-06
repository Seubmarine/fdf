/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_dup_edges.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 17:49:45 by tbousque          #+#    #+#             */
/*   Updated: 2022/06/24 17:49:48 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "del_dup_edges.h"

int	cmp_x(const void *i1, const void *i2)
{
	const t_edge	ei1 = *(t_edge *)i1;
	const t_edge	ei2 = *(t_edge *)i2;

	return (ei1.e[0] - ei2.e[0]);
}

int	cmp_y(const void *i1, const void *i2)
{
	const t_edge	ei1 = *(t_edge *)i1;
	const t_edge	ei2 = *(t_edge *)i2;

	return (ei1.e[1] - ei2.e[1]);
}

int	compare(t_edge a, t_edge b)
{
	return (a.e[0] == b.e[0] && a.e[1] == b.e[1]);
}

void	edge_rm(t_edge *src, size_t *edges_size, t_edge *buffer)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < *edges_size - 1)
	{
		if (!compare(src[i], src[i + 1]))
			buffer[j++] = src[i];
		i++;
	}
	buffer[j++] = src[*edges_size - 1];
	i = 0;
	while (i < j)
	{
		src[i] = buffer[i];
		i++;
	}
	*edges_size = j;
}

t_edge	*remove_edges_double(t_edge *edges, size_t *egdes_size)
{
	t_edge	*edges_buffer;

	edges_buffer = malloc(sizeof(*edges_buffer) * *egdes_size);
	if (edges_buffer == NULL)
		return (NULL);
	ft_qsort(edges, *egdes_size, sizeof(*edges), cmp_x);
	ft_qsort(edges, *egdes_size, sizeof(*edges), cmp_y);
	edge_rm(edges, egdes_size, edges_buffer);
	free(edges_buffer);
	return (edges);
}

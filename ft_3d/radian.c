/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   radian.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbousque <tbousque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 20:55:07 by tbousque          #+#    #+#             */
/*   Updated: 2022/06/23 21:02:47 by tbousque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "radian.h"

double	to_rad(double degree)
{
	return (degree * (3.14159f / 180.0));
}
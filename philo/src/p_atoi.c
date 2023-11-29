/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_atoi.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 16:17:38 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/11/29 17:03:30 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	p_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	check_string(char *str)
{
	int	i;

	i = 0;
	if (p_strlen(str) > 10)
		return (0);
	while (str[i])
	{
		if (str[i] < '0' && str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	p_atoi(char *str)
{
	if (!str || !check_string(str))
		return (-1);
	nb = 0;
	while (str)
	{
		nb = nb * 10 + (*str - '0');
		str++;
	}
	return (nb);
}

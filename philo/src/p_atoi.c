/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_atoi.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 16:17:38 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/11/30 13:56:40 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	p_strlen(char *str)
{
	int	i;

	i = 0;
	while (*str == ' ' || *str == '0')
		str++;
	while (*str)
	{
		i++;
		str++;
	}
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
	long long int	nb;

	if (!str || !check_string(str))
		return (-1);
	nb = 0;
	while (*str)
	{
		nb = nb * 10 + (*str - '0');
		str++;
	}
	if (nb > 2147483647 || nb < 0)
		return (-1);
	return (nb);
}

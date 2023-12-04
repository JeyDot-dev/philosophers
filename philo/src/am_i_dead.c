/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   am_i_dead.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 13:55:43 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/12/04 13:55:55 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	am_i_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->locks->l_is_eating[philo->id - 1]);
	if (philo->is_dead)
	{
		pthread_mutex_unlock(&philo->locks->l_is_eating[philo->id - 1]);
		return (1);
	}
	pthread_mutex_unlock(&philo->locks->l_is_eating[philo->id - 1]);
	return (0);
}

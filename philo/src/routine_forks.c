/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_forks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 13:35:59 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/12/04 13:39:48 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
int	dead_forks(t_philo *philo, pthread_mutex_t *forks, int mode)
{
	if (am_i_dead(philo))
	{
		if (mode == 1)
			pthread_mutex_unlock(&forks[philo->forks[1]]);
		else
		{
			pthread_mutex_unlock(&forks[philo->forks[0]]);
			pthread_mutex_unlock(&forks[philo->forks[1]]);
		}
		return (1);
	}
	return (0);
}
int routine_take_forks(t_philo *philo, struct timeval launch_time)
{
	if (am_i_dead(philo))
		return (1);
	pthread_mutex_lock(&philo->locks->l_forks[philo->forks[1]]);
	if (dead_forks(philo, philo->locks->l_forks, 1))
		return (1);
	pthread_mutex_lock(philo->locks->l_print);
	timestamp(launch_time, '1');
	printf("%d has taken a fork\n", philo->id);
	pthread_mutex_unlock(philo->locks->l_print);
	pthread_mutex_lock(&philo->locks->l_forks[philo->forks[0]]);
	if (dead_forks(philo, philo->locks->l_forks, 2))
		return (1);
	pthread_mutex_lock(philo->locks->l_print);
	timestamp(launch_time, '1');
	printf("%d has taken a fork\n", philo->id);
	pthread_mutex_unlock(philo->locks->l_print);
	if (dead_forks(philo, philo->locks->l_forks, 2))
		return (1);
	return (0);
}

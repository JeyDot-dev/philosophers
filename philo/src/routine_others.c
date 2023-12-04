/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_others.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 13:42:40 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/12/04 13:45:17 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	routine_sleep(t_philo *philo, struct timeval launch_time)
{
	if (am_i_dead(philo))
		return (1);
	pthread_mutex_lock(philo->locks->l_print);
	if (am_i_dead(philo))
	{
		pthread_mutex_unlock(philo->locks->l_print);
		return (1);
	}
	timestamp(launch_time, '1');
	printf("%d is sleeping\n", philo->id);
	pthread_mutex_unlock(philo->locks->l_print);
	super_sleep(launch_time, philo->parse.time_sleep);
	return (0);
}

int	routine_think(t_philo *philo, struct timeval launch_time)
{
	if (am_i_dead(philo))
		return (1);
	pthread_mutex_lock(philo->locks->l_print);
	if (am_i_dead(philo))
	{
		pthread_mutex_unlock(philo->locks->l_print);
		return (1);
	}
	timestamp(launch_time, '1');
	printf("%d is thinking\n", philo->id);
	pthread_mutex_unlock(philo->locks->l_print);
	return (0);
}

int	routine_eat(t_philo *philo, struct timeval launch_time)
{
	int	time_to_eat;
	if (am_i_dead(philo))
		return (1);
	pthread_mutex_lock(philo->locks->l_print);
	timestamp(launch_time, '1');
	printf("%d is eating\n", philo->id);
	pthread_mutex_unlock(philo->locks->l_print);
	pthread_mutex_lock(&philo->locks->l_is_eating[philo->id - 1]);
	time_to_eat = philo->parse.time_eat;
	philo->last_eat = timestamp(launch_time, '0') + time_to_eat;
	pthread_mutex_unlock(&philo->locks->l_is_eating[philo->id - 1]);
	super_sleep(launch_time, time_to_eat);
	pthread_mutex_unlock(&philo->locks->l_forks[philo->forks[0]]);
	pthread_mutex_unlock(&philo->locks->l_forks[philo->forks[1]]);
	pthread_mutex_lock(&philo->locks->l_is_eating[philo->id - 1]);
	philo->nb_eaten++;
	pthread_mutex_unlock(&philo->locks->l_is_eating[philo->id - 1]);
	return (0);
}

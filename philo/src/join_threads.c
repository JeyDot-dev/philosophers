/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 13:47:35 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/12/04 13:51:39 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	kill_philo(t_philo *philo)
{
	int	current_time;

	current_time = timestamp(philo->start, '0');
	if (current_time - philo->last_eat > philo->parse.time_die)
		philo->is_dead = 1;
}

int	check_death(t_philo *philo)
{
	kill_philo(philo);
	if (philo->is_dead)
	{
		pthread_mutex_lock(philo->locks->l_print);
		timestamp(philo->start, '1');
		printf("%d died\n", philo->id);
		pthread_mutex_unlock(philo->locks->l_print);
		pthread_mutex_unlock(&philo->locks->l_is_eating[philo->id - 1]);
		if (philo->parse.nb_philo == 1)
			pthread_mutex_unlock(&philo->locks->l_forks[philo->forks[0]]);
		return (1);
	}
	return (0);
}

void kill_and_check_nb_eaten(t_philo *philos, int nb_eaten, int nb_philos)
{
	int	i;

	i = 0;
	while (i < nb_philos)
	{
		pthread_mutex_lock(&philos->locks->l_is_eating[philos[i].id - 1]);
		philos[i].is_dead = 1;
		pthread_mutex_unlock(&philos->locks->l_is_eating[philos[i].id - 1]);
		i++;
	}
	if (nb_eaten == nb_philos)
	{
		pthread_mutex_lock(philos->locks->l_print);
		printf("All philosophers have eaten %d times\n", philos->parse.nb_eat);
		pthread_mutex_unlock(philos->locks->l_print);
	}
}

void check_and_kill_philos(t_philo *philos, int nb_philos)
{
	int	i;
	int	nb_eaten;

	i = 0;
	nb_eaten = 0;
	while (nb_eaten != nb_philos && !am_i_dead(&philos[i]))
	{
		pthread_mutex_lock(&philos->locks->l_is_eating[philos[i].id - 1]);
		if (check_death(&philos[i]))
			break ;
		if (philos[i].parse.nb_eat >= 0 && philos[i].nb_eaten >= philos[i].parse.nb_eat)
			nb_eaten++;
		pthread_mutex_unlock(&philos->locks->l_is_eating[philos[i].id - 1]);
		i++;
		if (i == nb_philos && nb_eaten != nb_philos)
		{
			i = 0;
			nb_eaten = 0;
		}
	}
	kill_and_check_nb_eaten(philos, nb_eaten, nb_philos);
}

void	join_threads(pthread_t *threads, t_philo *philos)
{
	int	i;
	int	nb_philos;

	i = 0;
	nb_philos = philos->parse.nb_philo;
	check_and_kill_philos(philos, nb_philos);
	while (i < nb_philos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free_and_destroy(philos, nb_philos);
	free(threads);
}

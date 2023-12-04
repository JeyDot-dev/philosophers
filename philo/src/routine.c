/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 13:31:54 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/12/04 14:16:53 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	free_and_destroy(t_philo *philos, int nb_philos)
{
	int	i;

	i = 0;
	while (i < nb_philos)
	{
		pthread_mutex_destroy(&philos->locks->l_forks[i]);
		pthread_mutex_destroy(&philos->locks->l_is_eating[i]);
		i++;
	}
	pthread_mutex_destroy(philos->locks->l_print);
	free(philos->locks->l_forks);
	free(philos->locks->l_is_eating);
	free(philos->locks->l_print);
	free(philos->locks);
	free(philos);
}

void	*do_routine(void *v_philo)
{
	t_philo		*philo;
	int			id;

	philo = (t_philo *) v_philo;
	id = philo->id;
	if (id % 2 == 1)
		super_sleep(philo->start, philo->parse.time_eat / 2);
	while (am_i_dead(philo) == 0)
	{
		routine_take_forks(philo, philo->start);
		routine_eat(philo, philo->start);
		routine_sleep(philo, philo->start);
		routine_think(philo, philo->start);
	}
	return (0);
}

void	routine(t_philo *philos)
{
	pthread_t		*threads;
	int				nb_philos;
	struct timeval	start;
	int				i;

	i = 0;
	nb_philos = philos->parse.nb_philo;
	threads = malloc(sizeof(pthread_t) * philos->parse.nb_philo);
	gettimeofday(&start, NULL);
	while (i < nb_philos)
	{
		philos[i].start = start;
		pthread_create(&threads[i], NULL, do_routine, &philos[i]);
		i ++;
	}
	join_threads(threads, philos);
}

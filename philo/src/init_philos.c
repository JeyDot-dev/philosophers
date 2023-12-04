/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 13:30:18 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/12/04 14:20:23 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

t_philo	*malloc_philos(int nb_philos)
{
	t_philo	*philos;
	t_locks	*locks;

	philos = malloc(sizeof(t_philo) * nb_philos);
	if (!philos)
		return (NULL);
	locks = malloc(sizeof(t_locks));
	if (!locks)
	{
		free(philos);
		return (NULL);
	}
	locks->l_forks = malloc(sizeof(pthread_mutex_t) * nb_philos);
	if (!locks->l_forks)
	{
		free(philos);
		free(locks);
		return (NULL);
	}
	locks->l_print = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(locks->l_print, NULL);
	philos->locks = locks;
	return (philos);
}

t_philo	*init_extension(t_philo *philos, t_parse parse, t_locks *locks)
{
	int	i;

	i = 0;
	while (i < parse.nb_philo)
	{
		philos[i].id = i + 1;
		philos[i].forks[0] = i;
		pthread_mutex_init(&locks->l_forks[philos[i].forks[0]], NULL);
		if (i == parse.nb_philo - 1)
			philos[i].forks[1] = 0;
		else
			philos[i].forks[1] = i + 1;
		philos[i].is_dead = 0;
		philos[i].nb_eaten = 0;
		philos[i].last_eat = 0;
		philos[i].locks = locks;
		philos[i].parse = parse;
		i++;
	}
	return (philos);
}

t_philo	*init_philos(t_parse parse)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = malloc_philos(parse.nb_philo);
	if (!philos)
		return (NULL);
	philos = init_extension(philos, parse, philos->locks);
	philos->locks->l_is_eating = malloc(sizeof(pthread_mutex_t)
			* parse.nb_philo);
	if (!philos->locks->l_is_eating)
	{
		free(philos);
		return (NULL);
	}
	while (i < parse.nb_philo)
		pthread_mutex_init(&philos->locks->l_is_eating[i++], NULL);
	return (philos);
}

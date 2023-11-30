/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:14:21 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/11/30 19:50:04 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
void	print_time_since_launch(struct timeval launch_time)
{
	struct timeval current_time;
	
	usleep(1 * 100000);
	gettimeofday(&current_time, NULL);
	printf("time since launch: %li\n", current_time.tv_sec * 1000 - launch_time.tv_sec * 1000 + current_time.tv_usec / 1000 - launch_time.tv_usec / 1000);
}
void	*thread_test(void *arg)
{
	t_philo	*philos;
	
	philos = (t_philo *) arg;
	while (1)
	{
		usleep(1 * 1000000);
		if (philos->is_dead)
			printf("Philosopher %d is dead\n", philos->id);
		else
			printf("Philosopher %d is alive\n", philos->id);
	}

}
t_philo *malloc_philos(int nb_philos)
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
	philos->locks = locks;
	return (philos);
}
t_philo *init_extension(t_philo *philos, t_parse parse, t_locks *locks)
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
		{
			philos[i].forks[1] = i + 1;
			pthread_mutex_init(&locks->l_forks[philos[i].forks[1]], NULL);
		}
		philos[i].is_dead = 0;
		philos[i].nb_eaten = 0;
		philos[i].locks = locks;
		philos[i].parse = parse;
		i++;
	}
	return (philos);
}
t_philo *init_philos(t_parse parse)
{
	t_philo	*philos;

	philos = malloc_philos(parse.nb_philo);
	if (!philos)
		return (NULL);
	philos = init_extension(philos, parse, philos->locks);
	return (philos);
}
void	print_philos(t_philo *philos, int nb_philos)
{
	int	i;

	i = 0;
	while (i < nb_philos)
	{
		printf("Philosopher %d\n", philos[i].id);
		printf("Forks: %d, %d\n", philos[i].forks[0], philos[i].forks[1]);
		i++;
	}
}
int	main(int ac, char **av)
{
	t_parse			parse;
	pthread_t		*threads;
	t_philo			*philos;
	struct timeval	launch_time;

	(void)launch_time;
	parse = set_user_input(ac, av);
	if (parse.error)
		return (1);
	threads = malloc(sizeof(pthread_t) * parse.nb_philo);
	(void)threads;
	philos = init_philos(parse);
	print_philos(philos, parse.nb_philo);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:14:21 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/12/02 20:15:09 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
long long	timestamp(struct timeval launch_time, char c)
{
	struct timeval	current_time;
	u_int64_t		time;
	
	gettimeofday(&current_time, NULL);
	time = current_time.tv_sec * 1000 - launch_time.tv_sec * 1000 + current_time.tv_usec / 1000 - launch_time.tv_usec / 1000;
	if (c == '1')
		printf("%li ", time);
	return ((int) time);
}
void	utimestamp(struct timeval launch_time, char *str)
{
	struct timeval current_time;
	
	gettimeofday(&current_time, NULL);
	if (str)
		printf("%s", str);
	printf("time since launch: %lius\n",
			current_time.tv_sec * 1000000 - launch_time.tv_sec * 1000000 + current_time.tv_usec - launch_time.tv_usec);
}
void free_and_destroy(t_philo *philos, int nb_philos)
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
void	super_sleep(struct timeval launch_time, long time)
{
	struct timeval current_time;
	
	(void) launch_time;
	gettimeofday(&current_time, NULL);
	time = current_time.tv_sec * 1000 + current_time.tv_usec / 1000 + time;
	while (current_time.tv_sec * 1000 + current_time.tv_usec / 1000 < time)
	{
		usleep(100);
		gettimeofday(&current_time, NULL);
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

int	join_threads(pthread_t *threads, t_philo *philos)
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
	return (0);
}

int	routine_sleep(t_philo *philo, struct timeval launch_time)
{
	if (am_i_dead(philo))
		return (0);
	pthread_mutex_lock(philo->locks->l_print);
	if (am_i_dead(philo))
	{
		pthread_mutex_unlock(philo->locks->l_print);
		return (0);
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
		return (0);
	pthread_mutex_lock(philo->locks->l_print);
	if (am_i_dead(philo))
	{
		pthread_mutex_unlock(philo->locks->l_print);
		return (0);
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
		return (0);
	pthread_mutex_lock(philo->locks->l_print);
	timestamp(launch_time, '1');
	printf("%d is eating\n", philo->id);
	pthread_mutex_unlock(philo->locks->l_print);
	pthread_mutex_lock(&philo->locks->l_is_eating[philo->id - 1]);
	time_to_eat = philo->parse.time_eat;
	philo->last_eat = timestamp(launch_time, '0') + time_to_eat;
	pthread_mutex_unlock(&philo->locks->l_is_eating[philo->id - 1]);
	super_sleep(launch_time, time_to_eat);
	pthread_mutex_lock(&philo->locks->l_is_eating[philo->id - 1]);
	philo->nb_eaten++;
	philo->last_eat = timestamp(launch_time, '0');
	pthread_mutex_unlock(&philo->locks->l_is_eating[philo->id - 1]);
	if (am_i_dead(philo))
	{
		pthread_mutex_unlock(&philo->locks->l_forks[philo->forks[0]]);
		pthread_mutex_unlock(&philo->locks->l_forks[philo->forks[1]]);
	}
	return (0);
}
int routine_take_forks(t_philo *philo, struct timeval launch_time)
{
	if (am_i_dead(philo))
		return (0);
	pthread_mutex_lock(&philo->locks->l_forks[philo->forks[1]]);
	if (am_i_dead(philo))
	{
		pthread_mutex_unlock(&philo->locks->l_forks[philo->forks[1]]);
		return (0);
	}
	pthread_mutex_lock(philo->locks->l_print);
	timestamp(launch_time, '1');
	printf("%d has taken a fork\n", philo->id);
	pthread_mutex_unlock(philo->locks->l_print);
	pthread_mutex_lock(&philo->locks->l_forks[philo->forks[0]]);
	pthread_mutex_lock(philo->locks->l_print);
	if (am_i_dead(philo))
	{
		pthread_mutex_unlock(&philo->locks->l_forks[philo->forks[0]]);
		pthread_mutex_unlock(&philo->locks->l_forks[philo->forks[1]]);
		return (0);
	}
	timestamp(launch_time, '1');
	printf("%d has taken a fork\n", philo->id);
	pthread_mutex_unlock(philo->locks->l_print);
	if (am_i_dead(philo))
	{
		pthread_mutex_unlock(&philo->locks->l_forks[philo->forks[0]]);
		pthread_mutex_unlock(&philo->locks->l_forks[philo->forks[1]]);
		return (0);
	}
	return (0);
}
int routine_put_forks(t_philo *philo)
{
	if (am_i_dead(philo))
		return (0);
	pthread_mutex_unlock(&philo->locks->l_forks[philo->forks[0]]);
	pthread_mutex_unlock(&philo->locks->l_forks[philo->forks[1]]);
	return (0);
}
void	*do_routine(void *v_philo)
{
	t_philo			*philo;
	int				id;

	philo = (t_philo *) v_philo;
	id = philo->id;
	if (id % 2 == 1)
		super_sleep(philo->start, philo->parse.time_eat / 2);
	while (am_i_dead(philo) == 0)
	{
//		if (id % 2 == 1)
//		{
//			routine_sleep(philo, philo->start);
//			routine_think(philo, philo->start);
//			routine_take_forks(philo, philo->start);
//			routine_eat(philo, philo->start);
//			routine_put_forks(philo);
//		}
//		else
//		{
			routine_take_forks(philo, philo->start);
			routine_eat(philo, philo->start);
			routine_put_forks(philo);
			routine_sleep(philo, philo->start);
			routine_think(philo, philo->start);
//		}
	}
	return (0);
}
int	routine(t_philo *philos)
{
	pthread_t	*threads;
	int			nb_philos;
	struct timeval	start;
	int			i;

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
	return (0);
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
	locks->l_print = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(locks->l_print, NULL);
	philos->locks = locks;
	return (philos);
}
t_philo *swap_forks(t_philo *philo, int i)
{
//	philo[i].forks[1] = 0;
	philo[i].forks[0] = 0;
	philo[i].forks[1] = i;
	return (philo);
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
			philos = swap_forks(philos, i);
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
t_philo *init_philos(t_parse parse)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = malloc_philos(parse.nb_philo);
	if (!philos)
		return (NULL);
	philos = init_extension(philos, parse, philos->locks);
	philos->locks->l_is_eating = malloc(sizeof(pthread_mutex_t) * parse.nb_philo);
	if (!philos->locks->l_is_eating)
	{
		free(philos);
		return (NULL);
	}
	while (i < parse.nb_philo)
		pthread_mutex_init(&philos->locks->l_is_eating[i++], NULL);
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
	t_philo			*philos;

	parse = set_user_input(ac, av);
	if (parse.error)
		return (1);
	philos = init_philos(parse);
	//print_philos(philos, parse.nb_philo);
	routine(philos);
	return (0);
}

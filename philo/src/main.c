/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:14:21 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/11/30 17:04:14 by jsousa-a         ###   ########.fr       */
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
int	main(int ac, char **av)
{
	t_parse	parse;
	pthread_t	threads[1];
	t_philo		philos[1];
	struct timeval launch_time;

	gettimeofday(&launch_time, NULL);
	philos->is_dead = 0;
	print_time_since_launch(launch_time);
	pthread_create(&threads[0], NULL, thread_test, (void *) philos);
	parse = set_user_input(ac, av);
	if (parse.error)
		return (1);
	usleep(5 * 1000000);
	print_time_since_launch(launch_time);
	philos->is_dead = 1;
	pthread_join(threads[0], NULL);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:16:26 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/12/04 14:23:39 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# define BLK "\e[0;30m"
# define RED "\e[0;31m"
# define GRN "\e[0;32m"
# define YEL "\e[0;33m"
# define BLU "\e[0;34m"
# define MAG "\e[0;35m"
# define CYN "\e[0;36m"
# define WHT "\e[0;37m"

typedef struct s_locks
{
	pthread_mutex_t	*l_forks;
	pthread_mutex_t	*l_is_eating;
	pthread_mutex_t	*l_is_dead;
	pthread_mutex_t	*l_nb_eaten;
	pthread_mutex_t	*l_last_eat;
	pthread_mutex_t	*l_print;
}				t_locks;

typedef struct s_parse
{
	int	nb_philo;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	nb_eat;
	int	error;
}				t_parse;

typedef struct s_philo
{
	int				id;
	int				forks[2];
	int				nb_eaten;
	int				is_eating;
	long long		last_eat;
	int				is_dead;
	struct timeval	start;
	t_locks			*locks;
	t_parse			parse;
}				t_philo;

t_parse		set_user_input(int ac, char **av);
int			p_atoi(char *str);
t_philo		*init_philos(t_parse parse);
void		routine(t_philo *philos);
void		super_sleep(struct timeval launch_time, long time);
long long	timestamp(struct timeval launch_time, char c);
int			routine_take_forks(t_philo *philo, struct timeval launch_time);
int			routine_eat(t_philo *philo, struct timeval launch_time);
int			routine_think(t_philo *philo, struct timeval launch_time);
int			routine_sleep(t_philo *philo, struct timeval launch_time);
void		join_threads(pthread_t *threads, t_philo *philos);
int			am_i_dead(t_philo *philo);
void		free_and_destroy(t_philo *philos, int nb_philos);
#endif

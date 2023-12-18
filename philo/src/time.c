/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 13:34:53 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/12/18 15:12:13 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

long long	timestamp(struct timeval launch_time, char c)
{
	struct timeval	current_time;
	u_int64_t		time;

	gettimeofday(&current_time, NULL);
	time = current_time.tv_sec * 1000 - launch_time.tv_sec
		* 1000 + current_time.tv_usec / 1000 - launch_time.tv_usec / 1000;
	if (c == '1')
		printf("%llu ", time);
	return ((int) time);
}

void	super_sleep(struct timeval launch_time, long time)
{
	struct timeval	current_time;

	(void) launch_time;
	gettimeofday(&current_time, NULL);
	time = current_time.tv_sec * 1000 + current_time.tv_usec / 1000 + time;
	while (current_time.tv_sec * 1000 + current_time.tv_usec / 1000 < time)
	{
		usleep(100);
		gettimeofday(&current_time, NULL);
	}
}

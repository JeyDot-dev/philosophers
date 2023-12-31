/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 12:14:21 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/12/04 13:56:15 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	main(int ac, char **av)
{
	t_parse			parse;
	t_philo			*philos;

	parse = set_user_input(ac, av);
	if (parse.error)
		return (1);
	philos = init_philos(parse);
	routine(philos);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_user_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 17:48:05 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/11/29 18:13:42 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	print_error(char *str)
{
	printf("Error: %s\n", str);
	return (1);
}

t_parse	set_user_extension(int ac, char **av, int i, t_parse parse)
{
	parse.error = 0;
	parse.nb_philo = p_atoi(av[i++]);
	if (parse.nb_philo < 1)
		parse.error = print_error("Wrong number of philosophers");
	parse.time_die = p_atoi(av[i++]);
	if (parse.time_die < 1)
		parse.error = print_error("Wrong time to die");
	parse.time_eat = p_atoi(av[i++]);
	if (parse.time_eat < 1)
		parse.error = print_error("Wrong time to eat");
	parse.time_sleep = p_atoi(av[i++]);
	if (parse.time_sleep < 1)
		parse.error = print_error("Wrong time to sleep");
	if (ac == 6)
	{
		parse.nb_eat = p_atoi(av[i++]);
		if (parse.nb_eat < 1)
			parse.error = print_error("Wrong number of times to eat");
	}
	else
		parse.nb_eat = -1;
	return (parse);
}

t_parse	set_user_input(int ac, char **av)
{
	t_parse	parse;
	int		i;

	i = 1;
	if (ac < 5 || ac > 6)
	{
		printf("Error: Wrong number of arguments\n");
		parse.error = 1;
		return (parse);
	}
	parse = set_user_extension(ac, av, i, parse);
	return (parse);
}
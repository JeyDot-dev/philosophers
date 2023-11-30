/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_user_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsousa-a <jsousa-a@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 17:48:05 by jsousa-a          #+#    #+#             */
/*   Updated: 2023/11/30 16:16:15 by jsousa-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	print_error(char *str)
{
	printf("Error: %s\n", str);
	return (1);
}

void	check_time_coherence(t_parse parse)
{
	int	time_to_eat_total;

	time_to_eat_total = parse.time_eat;
	if (parse.nb_philo > 1 && parse.nb_philo % 2 == 1)
		time_to_eat_total = parse.time_eat * 2;
	if (parse.time_die < time_to_eat_total + parse.time_sleep)
		print_error("Time to die is too short (or time to eat/sleep is too long)");
}
void	print_parse(t_parse parse, int ac)
{
	if (ac == 6)
	{
		printf("|\tnb phils\t|\ttime to die\t|\ttime to eat\t|\ttime to sleep\t|\tnb eat\t|\n");
		printf("|\t%d\t\t|\t%d\t\t|\t%d\t\t|\t%d\t\t|\t%d\t|\n", parse.nb_philo, parse.time_die,
		parse.time_eat, parse.time_sleep, parse.nb_eat);
	}
	else
	{
		printf("|\tnb phils\t|\ttime to die\t|\ttime to eat\t|\ttime to sleep\t|\n");
		printf("|\t%d\t\t|\t%d\t\t|\t%d\t\t|\t%d\t\t|\n", parse.nb_philo, parse.time_die,
		parse.time_eat, parse.time_sleep);
	}
}

t_parse	set_user_extension(int ac, char **av, int i, t_parse parse)
{
	parse.error = 0;
	parse.nb_philo = p_atoi(av[++i]);
	if (parse.nb_philo < 1)
		parse.error = print_error("Wrong number of philosophers");
	parse.time_die = p_atoi(av[++i]);
	if (parse.time_die < 1)
		parse.error = print_error("Wrong time to die");
	parse.time_eat = p_atoi(av[++i]);
	if (parse.time_eat < 1)
		parse.error = print_error("Wrong time to eat");
	parse.time_sleep = p_atoi(av[++i]);
	if (parse.time_sleep < 1)
		parse.error = print_error("Wrong time to sleep");
	if (ac == 6)
	{
		parse.nb_eat = p_atoi(av[++i]);
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

	i = 0;
	if (ac < 5 || ac > 6)
	{
		printf("Error: Wrong number of arguments\n");
		parse.error = 1;
		return (parse);
	}
	parse = set_user_extension(ac, av, i, parse);
	if (!parse.error)
		print_parse(parse, ac);
	return (parse);
}

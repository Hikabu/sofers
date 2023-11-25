/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 10:36:24 by valeriafedo       #+#    #+#             */
/*   Updated: 2023/11/25 19:09:58 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	message(char *str, t_philo *philo)
{
	sem_wait(philo->print);
	// if (!(is_dead(philo)))
	printf("%llu philo[%d] %s\n", get_time(), philo->id, str);
	sem_post(philo->print);
}

// int	mb_philo_dead(t_philo *data)
// {
// 	int	i;

// 	i = -1;
// 	while (++i < data->nbr_philo)
// 	{
// 		pthread_mutex_lock(&(data->philo[i].last_eat));
// 		if (get_time() - data->philo[i].long_last_eat > data->die_tm)
// 		{
// 			pthread_mutex_lock(&data->mutex_dead);
// 			data->dead = 1;
// 			pthread_mutex_unlock(&data->mutex_dead);
// 			pthread_mutex_lock(&data->mutex_print);
// 			printf("%llu philo[%d] died\n", get_time()
// 				- data->philo[i].long_last_eat, data->philo[i].id);
// 			pthread_mutex_unlock(&data->mutex_print);
// 			pthread_mutex_unlock(&(data->philo[i].last_eat));
// 			return (1);
// 		}
// 		pthread_mutex_unlock(&(data->philo[i].last_eat));
// 	}
// 	return (0);
// }

int	god_safe_philo(t_philo *data)
{
	usleep(100);
	sem_wait(data->time);
	if (get_time() - data->last_meal > data->die_tm)
	{
		data->dead = 1;
		sem_wait(data->print);
		printf("%llu philo[%d] died\n", get_time()
			- data->last_meal, data->id);
		// sem_post(data->time);
		data->stop = 1;
		return (1);
	}
	sem_post(data->time);
	sem_post(data->place);
	if (data->long_last_eat != -1 && data->cnt_meal == data->long_last_eat)
	{
		data->stop = 1;
		sem_wait(data->print);
		printf("this is it\n");
		return (1);
	}
	sem_post(data->place);
	return (0);
}

void	ft_destroy(t_philo *data)
{
	int	i;
	int	flag;

	i = -1;
	waitpid(-1, &flag, 0);
	if (flag != 0)
	{
		i = -1;
		while (++i < data->nbr_philo && data->fid[i])
			kill(data->fid[i], SIGKILL);
	}
	sem_close(data->fork);
	sem_close(data->print);
	sem_close(data->place);
	sem_close(data->time);
	sem_unlink("fork");
	sem_unlink("print");
	sem_unlink("place");
	sem_unlink("time");
	exit(1);
}

void	is_error(char *str)
{
	printf("%s\n", str);
	exit(1);
}
int	main(int ac, char **av)
{
	t_philo	data;
	t_philo *philo;
	int		i;

	i = -1;
	get_time();
	if (ac == 5 || ac == 6)
	{
		if (!pars(&data, av))
			return (printf("wrong arguments\n"));
		pars(&data, av);
		init_1(&data, av);
		init_semafor(&data);
		philo = &data;
		while (++i < data.nbr_philo)
		{
			philo->fid[i] = fork();
			if (philo->fid[i] == -1)
				is_error("Error with fork\n");
			else if (philo->fid[i] == 0)
			{
				philo->id = i + 1;
				init_philo(philo);
			}
		}
	}
	else
		return (printf("wrong arguments\n"));
	ft_destroy(&data);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 23:25:11 by valeriafedo       #+#    #+#             */
/*   Updated: 2023/11/25 19:10:28 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// void	eating(t_philob *philo)
// {
// 	printing(philo, "is eating");
// 	my_usleep(philo->time_to_eat);
// 	if (philo->notepme != -1 && philo->eating_count < philo->notepme)
// 	{
// 		sem_wait(philo->ceat);
// 		philo->eating_count++;
// 		sem_post(philo->ceat);
// 	}
// 	sem_wait(philo->time);
// 	philo->last_eating = time_now();
// 	sem_post(philo->time);
// }

void	eating(t_philo *philo)
{
	message(EAT, philo);
	mysleep(philo->eat_tm);
	philo->cnt_meal++;
	philo->last_meal = get_time();
}
void	init_philo(t_philo *data)
{
	if (pthread_create(&data->thread, NULL, &routine, data))
		is_error(TH_CREATE);
	if (data->id % 2)
		usleep(100);
	while (1)
	{
		sem_wait(data->fork);
		message(TAKE_FORK, data);
		sem_wait(data->fork);
		message(TAKE_FORK, data);
		eating(data);
		sem_post(data->fork);
		sem_post(data->fork);
		message(SLEEP, data);
		mysleep(data->sleep_tm);
	}
	if (pthread_join(data->thread, NULL))
		is_error(TH_JOIN);
}

int	init_semafor(t_philo *data)
{
	int	i;

	i = -1;
	while (++i < data->nbr_philo)
		data->fid[i] = -1;
	data->cnt_meal = 0;
	data->last_meal = 0;
	data->dead = 0;
	data->stop = 0;
	sem_unlink("fork");
	sem_unlink("print");
	sem_unlink("place");
	sem_unlink("time");
	data->print = sem_open("print", O_CREAT, 0644, 1);
	data->fork = sem_open("fork", O_CREAT, 0644, data->nbr_philo);
	data->place = sem_open("place", O_CREAT, 0644, 1);
	data->time = sem_open("timr", O_CREAT, 0644, 1);
	if (data->print <= 0 || data->fork <= 0
		|| data->place <= 0 || data->time <= 0)
	{
		printf("Error with semaphores\n");
		return (1);
	}
	return (0);
}

int	init_1(t_philo *data, char **av)
{
	data->dead = 0;
	data->nbr_philo = ft_atol(av[1]);
	if (data->nbr_philo == 0)
		return (1);
	data->die_tm = ft_atol(av[2]);
	data->eat_tm = ft_atol(av[3]);
	data->sleep_tm = ft_atol(av[4]);
	if (av[5])
		data->long_last_eat = ft_atol(av[5]);
	else
		data->long_last_eat = -1;
	return (0);
}

// int	is_dead(t_philo *philo)
// {
// 	int	i;

// 	pthread_mutex_lock(philo->mutex_dead);
// 	i = *(philo->die);
// 	pthread_mutex_unlock(philo->mutex_dead);
// 	return (i);
// }

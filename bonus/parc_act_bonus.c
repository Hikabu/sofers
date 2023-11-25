/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parc_act_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valeriafedorova <valeriafedorova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 18:11:53 by valeriafedo       #+#    #+#             */
/*   Updated: 2023/11/25 17:52:06 by valeriafedo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*routine(void *smt)
{
	t_philo	*philo;

	philo = (t_philo *)smt;
	while (!philo->stop)
		if (god_safe_philo(philo))
			break;
	if (philo->dead)
		exit(1);
	else if (philo->stop)
		exit(1);
	else
		exit(0);
}

int	pars(t_philo *data, char **av)
{
	int	i;
	
	(void)data;
	i = 1;
	while (av[i])
	{
		if (ft_atol(av[1]) > 200)
			return (0);
		if (!ft_strisnum(av[i]))
			return (0);
		i++;
	}
	return (1);
}

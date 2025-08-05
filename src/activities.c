/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcorlett <rcorlett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:09:51 by rcorlett          #+#    #+#             */
/*   Updated: 2025/03/11 11:05:21 by rcorlett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sleeping(t_philo *philo)
{
	if (!is_dead(philo) && philo->status != SLEEPING) //Verifies if he is dead or if he has the status of sleeping)
	{
		philo->status = SLEEPING; //Set the status to Sleeping
		logs(philo, philo->status); //Print the status
		wait_time(philo, philo->data->t_sleep); //Wait for the time configure to sleep
	}
}

void	thinking(t_philo *philo)
{
	if (!is_dead(philo) && philo->status != THINKING)
	{
		philo->status = THINKING;
		logs(philo, philo->status);
		wait_time(philo, 5); //Fix time to think, avoiding the philo thinks too much and starves
	}
}

static bool	get_fork(t_philo *philo, int fork)
{
	pthread_mutex_lock(philo->fork[fork]); //Tries to block the choosen fork
	if (is_dead(philo)) //if the philo dies in the process, unlock the mutex
	{
		pthread_mutex_unlock(philo->fork[fork]);
		return (false);
	}
	logs(philo, FORK); //If not, store the info about the chosen fork
	return (true);
}

static bool	get_forks(t_philo *philo) //Tries to take the left fork, tries to take the right fork, if fails, free the left fork to the  other, if gets both, it is true
{
	if (!get_fork(philo, LEFT)) 
		return (false);
	if (!get_fork(philo, RIGHT))
	{
		pthread_mutex_unlock(philo->fork[LEFT]);
		return (false);
	}
	return (true);
}

void	eating(t_philo *philo)
{
	if (!get_forks(philo)) //Tries to take the forks, if fails return ERROR
		return ;
	pthread_mutex_lock(&philo->lock); //Block the philo to update the time limit of death
	philo->t_death = get_time() + philo->data->t_death;
	pthread_mutex_unlock(&philo->lock);
	philo->status = EATING; //Set the status and stored in log
	logs(philo, philo->status);
	philo->meals++; //Increment the number of meals
	wait_time(philo, philo->data->t_eat); //Waits the time to eat
	pthread_mutex_unlock(philo->fork[LEFT]); //Free the fork
	pthread_mutex_unlock(philo->fork[RIGHT]); //Free the fork
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcorlett <rcorlett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:12:45 by rcorlett          #+#    #+#             */
/*   Updated: 2025/03/11 11:05:05 by rcorlett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	logs(t_philo *p, int status)
{
	pthread_mutex_lock(&p->data->log); //Block the lock for each thread write one by one
	if (status == DEATH) //If death, prints the time of start and identifies the philo dead
		printf("%lu %lu died\n", get_time() - p->data->t_start, p->id);
	else if (status == EATING) //Print the time, ID, and say that is eating
		printf("%lu %lu is eating\n", get_time() - p->data->t_start, p->id);
	else if (status == SLEEPING) //Print the time, ID, and say that is sleeping
		printf("%lu %lu is sleeping\n", get_time() - p->data->t_start, p->id);
	else if (status == THINKING) //Print the time, ID, and say that is thinking
		printf("%lu %lu is thinking\n", get_time() - p->data->t_start, p->id);
	else if (status == FORK) //Print the time, ID, and say that take the fork
		printf("%lu %lu has taken a fork\n",
			get_time() - p->data->t_start, p->id);
	pthread_mutex_unlock(&p->data->log); //Unlock to other thread writes in the log
}

void	*routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *) philo_ptr; // Convert the point to identify as philo
	pthread_mutex_lock(&philo->lock); //Block the mutex of the philo to protect his data
	philo->t_death = philo->data->t_start + philo->data->t_death; //Define the limit of time for death if the philo does not eat
	pthread_mutex_unlock(&philo->lock); //Unlock the mutex of the philo
	if (philo->id % 2 == 0 || philo->id == philo->data->n_philos) //If the philo has the ID pair or is the last start thinking
		thinking(philo);
	if (is_solo(philo)) //Verifies is is just one philo, he only takes one fork and finishes the thread because he dies
		return (NULL); //Return ERROR
	while (!is_dead(philo)) //While the phino is not dead, his status is changing in that one
	{
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}

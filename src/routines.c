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
	pthread_mutex_lock(&p->data->log);
	if (status == DEATH)
		printf("%lu %lu died\n", get_time() - p->data->t_start, p->id);
	else if (status == EATING)
		printf("%lu %lu is eating\n", get_time() - p->data->t_start, p->id);
	else if (status == SLEEPING)
		printf("%lu %lu is sleeping\n", get_time() - p->data->t_start, p->id);
	else if (status == THINKING)
		printf("%lu %lu is thinking\n", get_time() - p->data->t_start, p->id);
	else if (status == FORK)
		printf("%lu %lu has taken a fork\n",
			get_time() - p->data->t_start, p->id);
	pthread_mutex_unlock(&p->data->log);
}

void	*routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *) philo_ptr;
	pthread_mutex_lock(&philo->lock);
	philo->t_death = philo->data->t_start + philo->data->t_death;
	pthread_mutex_unlock(&philo->lock);
	if (philo->id % 2 == 0 || philo->id == philo->data->n_philos)
		thinking(philo);
	if (is_solo(philo))
		return (NULL);
	while (!is_dead(philo))
	{
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}

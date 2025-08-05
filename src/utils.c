/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcorlett <rcorlett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:13:09 by rcorlett          #+#    #+#             */
/*   Updated: 2025/03/11 11:17:17 by rcorlett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_time(t_philo *philo, u_int64_t time) //Verifies if the waiting time is no longer than the time of death
{
	if ((get_time() + time) >= philo->t_death)
		usleep((philo->t_death - get_time()) * 1000);
	else
		usleep(time * 1000); //Makes the thread "dormir" to avoid the conflict
}

u_int64_t	get_time(void) //Take the time os the system to show on the display
{
	struct timeval	t;

	if (gettimeofday(&t, NULL))
	{
		printf("ERROR: gettimeofday()");
		return (0);
	}
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000)); //Convert in milliseconds
}

long	ft_atol(const char *nptr) //Convert a string into an integer when avoiding signals and characters
{
	long	sign;
	long	result;
	int		i;

	i = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	sign = 1;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	result = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}

void	free_data(t_data *data) //Free all memory
{
	u_int64_t	i;

	if (!data) //If the pointer fails (points to nowhere) return ERROR
		return ;
	i = -1;
	while (++i < data->n_philos) //Init util vcerifies the last philo
	{
		pthread_mutex_destroy(&data->forks[i]); //Destroy the mutex for the fork to free the resources 
		pthread_mutex_destroy(&data->philos[i].lock); //Destroy the mutex for the philo to free the resources 
	}
	pthread_mutex_destroy(&data->lock); //Destroy the global mutexes that protect the shared resources, and the logs
	pthread_mutex_destroy(&data->log);
	pthread_mutex_destroy(&data->finish_lock);
	if (data->table)
		free(data->table); //Free the pointers the the stored structed
	if (data->philos)
		free(data->philos);
	if (data->forks)
		free(data->forks);
	free(data); //Free the main structure
}

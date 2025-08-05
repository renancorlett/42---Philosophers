/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcorlett <rcorlett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:12:24 by rcorlett          #+#    #+#             */
/*   Updated: 2025/03/11 11:05:02 by rcorlett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	reunion(t_data *data) //Function that starts all threads and waits until the end
{
	u_int64_t	n; //Counter to pass to the philos

	data->t_start = get_time(); //remeber the time os the simulation starts
	if (data->t_start == 0) //If fails return ERROR
		return (1);
	pthread_mutex_lock(&data->lock); //Block the mutex to syncronize the creation of threads
	n = 0;
	while (n < data->n_philos) //For each philo
	{
		if (pthread_create(&data->table[n], NULL, &routine, &data->philos[n])) //Create the thread and executes the routine for each philo
			return (1); //If any creation of thread fails, returns ERROR
		n++;
	}
	pthread_mutex_unlock(&data->lock); //Unblock the the mutex after the thread creation
	n = 0;
	while (n < data->n_philos) //For each philo tread created
	{
		if (pthread_join(data->table[n], NULL)) //Wait the thread finish before to continue
			return (1); //If waiting fails returns ERROR
		n++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data		*data;

	if ((ac != 5 && ac != 6) || check_input(av)) //Verifies if the number of arguments are correct
	{
		printf("Error: Invalid arguments\n");
		return (0);
	}
	data = init_data(ac, av); //Init the structure with the recives data
	if (!data) //If the init fails, free and return ERROR
	{
		free_data(data);
		return (1);
	}
	reunion(data); //If not fails, execute the philo threads and wait until the end
	free_data(data);
}

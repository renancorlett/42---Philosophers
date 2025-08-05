/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcorlett <rcorlett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:12:09 by rcorlett          #+#    #+#             */
/*   Updated: 2025/03/11 11:04:53 by rcorlett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Data->Threads->Philos->Forks
//This function avoids deadlocks(disputes for the same resources). Every philo wants to take the left fork and will starve
//Thread is a line of execution inside the program using shared memory and resources (faster and lighter), doesn't need to be copied for the main process - Need for synchronization
//Fork is a new process(child process) copied from the (father process) and has its own memory, can execute different tasks (slowly and heavily) - No need for synchronization
//Mutex is a mechanism that controls access to the shared resource, ensuring that only one thread uses the resource at a time, avoiding data race
//Data race is when two or more threads try to access the same resources and modify this data; the final data could be wrong if this process is not handled

#include "philo.h"

static void	init_forks(t_data *data, u_int64_t i) //Init pointers to forks for each philo in circ. way
{
	while (i < (data->n_philos - 1)) //Pass for all philo except the last.
	{
		if (i % 2) //If odd(impar) Usual pattern (take the left fork) index before it
		{
			data->philos[i].fork[LEFT] = &data->forks[i - 1]; //Select the left fork and it is the previous index
			data->philos[i].fork[RIGHT] = &data->forks[i]; //Select the right fork and it is the actual index
		}
		else //If pair(par) The fork is immediately the next one, index after it
		{
			data->philos[i].fork[LEFT] = &data->forks[i]; //Select the left fork and it is the actual index
			data->philos[i].fork[RIGHT] = &data->forks[i + 1]; //Select the right fork and it is the index + one position
		}
		i++;
	}
	if (i % 2) //If odd(impar). Special treatment for the last philo
	{
		data->philos[i].fork[LEFT] = &data->forks[i - 1]; //Select the left fork and it is the previous index
		data->philos[i].fork[RIGHT] = &data->forks[i]; //Select the right fork and it is the actual index
	}
	else //If pair(par) The fork is immediately the next one, index after it
	{
		data->philos[i].fork[LEFT] = &data->forks[i]; //Select the left fork and it is the actual index
		data->philos[i].fork[RIGHT] = &data->forks[0]; //Select the right fork, and it is the first fork of the list
	}
}

static void	init_philos(t_data *data) //Init the philos
{
	u_int64_t	i; //Counter

	i = -1;
	while (++i < data->n_philos) //SUM and verifies if the number of philos got to the limit
	{
		data->philos[i].data = data; //Philo pointer to global data to access common config
		data->philos[i].id = i + 1; //Defines the philo ID in "1" for better understanding
		data->philos[i].meals = 0; //Init the meals counter in 0
		data->philos[i].status = -1; //Defines the status with (none action)
		data->philos[i].full = false; //Verifies if none philo is full
		data->philos[i].t_death = data->t_death; //Attributes the limit of time that a philo can be without eating
		pthread_mutex_init(&data->philos[i].lock, NULL); //Init mutex to the philo for protect his data
	}
	init_forks(data, 0); //After init the philos, init the pointers to each fork
}

static t_data	*init_threads(t_data *data) //Init threads and mutex for each philo and fork
{
	u_int64_t	i;

	data->finish = false;  //Verifies if the simulation is already finished
	data->philos_full = 0; //Verifies if none philo is full
	data->table = malloc(data->n_philos * sizeof(pthread_t)); //Memory allocation to the array of threads for each philo
	data->forks = malloc(data->n_philos * sizeof(pthread_mutex_t)); //Memory allocation to the array of mutex for each fork
	if (!data->table || !data->forks)
		return (NULL); //If the allocation fails, return ERROR
	pthread_mutex_init(&data->lock, NULL); //Init global mutex that controls shared variables and log, blocking each one
	pthread_mutex_init(&data->log, NULL);  //Block the logs to avoid mixed messages
	pthread_mutex_init(&data->finish_lock, NULL); //Protect the variable avoid readingg/writing at simuntaneous
	i = 0;
	while (i < data->n_philos) //Init the fork mutex individualy for each philo
		pthread_mutex_init(&data->forks[i++], NULL); //Null paremeter is for use the pattern conf. of the system
	return (data); //Return the structure already for use
}

t_data	*init_data(int ac, char **av) //Init the strcture of data
{
	t_data	*data; //Pointer to the structure and save all data about everything

	data = malloc(sizeof(t_data)); //Allocs memory, if fails return ERROR.
	if (!data)
		return (NULL);
	data->n_philos = ft_atol(av[1]); //Convert all strings to integer to pass the arguments at the terminal
	data->t_death = ft_atol(av[2]);
	data->t_eat = ft_atol(av[3]);
	data->t_sleep = ft_atol(av[4]);
	if (ac == 6) //Verifies if has an extra argument
		data->n_meals = ft_atol(av[5]); //Refers to the number of meals and transforms into an integer too
	else
		data->n_meals = 0; //If there isn't the extra argument, ignore it and start the simulation
	if (!data->n_philos || !data->t_death || !data->t_eat || !data->t_sleep) //Verifies if any value is 0, if it is, invalidate the start of simulation
	{
		free(data);
		return (NULL);
	}
	data->t_start = 0; //Init the time of the simulation with 0
	data->philos = malloc(data->n_philos * sizeof(t_philo)); //Alloc memory to the philos
	if (!data->philos) //If fails returns ERROR
		return (NULL);
	data = init_threads(data); //Init threads, mutex, forks, and structures
	init_philos(data); //init the philo individualy
	return (data); //Return point to the structure ready to use
}

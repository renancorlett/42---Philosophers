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

#include "../includes/philo.h"

static void	init_forks(t_data *data, u_int64_t i)
{
	while (i < (data->n_philos - 1))
	{
		if (i % 2)
		{
			data->philos[i].fork[LEFT] = &data->forks[i - 1];
			data->philos[i].fork[RIGHT] = &data->forks[i];
		}
		else
		{
			data->philos[i].fork[LEFT] = &data->forks[i];
			data->philos[i].fork[RIGHT] = &data->forks[i + 1];
		}
		i++;
	}
	if (i % 2)
	{
		data->philos[i].fork[LEFT] = &data->forks[i - 1];
		data->philos[i].fork[RIGHT] = &data->forks[i];
	}
	else
	{
		data->philos[i].fork[LEFT] = &data->forks[i];
		data->philos[i].fork[RIGHT] = &data->forks[0];
	}
}

static void	init_philos(t_data *data)
{
	u_int64_t	i;

	i = -1;
	while (++i < data->n_philos)
	{
		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].meals = 0;
		data->philos[i].status = -1;
		data->philos[i].full = false;
		data->philos[i].t_death = data->t_death;
		pthread_mutex_init(&data->philos[i].lock, NULL);
	}
	init_forks(data, 0);
}

static t_data	*init_threads(t_data *data)
{
	u_int64_t	i;

	data->finish = false;
	data->philos_full = 0;
	data->table = malloc(data->n_philos * sizeof(pthread_t));
	data->forks = malloc(data->n_philos * sizeof(pthread_mutex_t));
	if (!data->table || !data->forks)
		return (NULL);
	pthread_mutex_init(&data->lock, NULL);
	pthread_mutex_init(&data->log, NULL);
	pthread_mutex_init(&data->finish_lock, NULL);
	i = 0;
	while (i < data->n_philos)
		pthread_mutex_init(&data->forks[i++], NULL);
	return (data);
}

t_data	*init_data(int ac, char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->n_philos = ft_atol(av[1]);
	data->t_death = ft_atol(av[2]);
	data->t_eat = ft_atol(av[3]);
	data->t_sleep = ft_atol(av[4]);
	if (ac == 6)
		data->n_meals = ft_atol(av[5]);
	else
		data->n_meals = 0;
	if (!data->n_philos || !data->t_death || !data->t_eat || !data->t_sleep)
	{
		free(data);
		return (NULL);
	}
	data->t_start = 0;
	data->philos = malloc(data->n_philos * sizeof(t_philo));
	if (!data->philos)
		return (NULL);
	data = init_threads(data);
	init_philos(data);
	return (data);
}

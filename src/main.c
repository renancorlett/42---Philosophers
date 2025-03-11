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

#include "../includes/philo.h"

static int	reunion(t_data *data)
{
	u_int64_t	n;

	data->t_start = get_time();
	if (data->t_start == 0)
		return (1);
	pthread_mutex_lock(&data->lock);
	n = 0;
	while (n < data->n_philos)
	{
		if (pthread_create(&data->table[n], NULL, &routine, &data->philos[n]))
			return (1);
		n++;
	}
	pthread_mutex_unlock(&data->lock);
	n = 0;
	while (n < data->n_philos)
	{
		if (pthread_join(data->table[n], NULL))
			return (1);
		n++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data		*data;

	if ((ac != 5 && ac != 6) || check_input(av))
	{
		printf("Error: Invalid arguments\n");
		return (0);
	}
	data = init_data(ac, av);
	if (!data)
	{
		free_data(data);
		return (1);
	}
	reunion(data);
	free_data(data);
}

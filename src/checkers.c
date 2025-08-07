/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcorlett <rcorlett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:10:31 by rcorlett          #+#    #+#             */
/*   Updated: 2025/03/11 09:10:31 by rcorlett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	is_full(t_philo	*philo)
{
	if (philo->meals >= philo->data->n_meals //Verifies if the philo meals >= number of meals needed
		&& philo->data->n_meals != 0 && !philo->full) //There is no limit on meals, and the philo was not marked as full
	{
		philo->data->philos_full++; //Increment the philo that achieves the status of FULL
		philo->full = true; //Marks the philo as full
	}
}

bool	is_dead(t_philo *philo)
{
	bool	dead;

	dead = false; //Init saying that the philo is not dead
	pthread_mutex_lock(&philo->lock); //Block the philo for protection
	pthread_mutex_lock(&philo->data->finish_lock);//Block the log philo for protection
	if (get_time() >= philo->t_death || philo->data->finish) //Verifies if it passes the time of the death and if the simulation was marked as finish
	{
		if (!philo->data->finish) //If is not marked as finish
			logs(philo, DEATH); //Calls log to register death
		philo->data->finish = true; //Marks the end of the simulation
	}
	pthread_mutex_lock(&philo->data->lock); //Block another mutex for execution of is_full
	is_full(philo);
	if (philo->data->philos_full == philo->data->n_philos) //Verifies if the number of philos is equal to the full philos
		philo->data->finish = true; //If is, mark as true
	dead = philo->data->finish; //Define dead as actual value for finish
	pthread_mutex_unlock(&philo->data->lock); //Unblock in the reverse way
	pthread_mutex_unlock(&philo->data->finish_lock);
	pthread_mutex_unlock(&philo->lock);
	return (dead); //Return dead if the philo is dead or end of the simulation
}

bool	is_solo(t_philo *philo)
{
	if (philo->data->n_philos == 1) //Verifies if there is only one philo
	{
		pthread_mutex_lock(philo->fork[RIGHT]); //Block the right fork
		logs(philo, FORK); //Prints the log "... has taken the fork"
		pthread_mutex_unlock(philo->fork[RIGHT]); //Unlock the mutex
		logs(philo, THINKING); //Print the log thinking
		while (!is_dead(philo)) //Enter in the infinite loop
			;
		return (true); //Return true, saying that he is alone
	}
	return (false); //False if he is not alone
}

int	check_input(char **av)
{
	int		i;
	int		j;
	long	temp;

	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (av[i][j] == '-')
				return (1); //Return Error if find "-" Can't accept negative numbers
			if (av[i][j] == '+') //Pass to the next character if find "+'
				j++;
			if (av[i][j] && (av[i][j] < '0' || av[i][j] > '9')) //Verifies if is a character different of numbers
				return (1); //If has, return ERROR
		}
		temp = ft_atol(av[i]); //Change the array into an integer
		if (temp < 0 || temp > 4294967295) //Verifies if if fewer of bigger than the limit of 32bits if is return ERROR
			return (1);
	}
	if (av[2][0] == '0' && !av[2][1]) //Verifies if the time of death is "0" if it is, return ERROR
		return (1);
	return (0);
}



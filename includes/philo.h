/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcorlett <rcorlett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:13:47 by rcorlett          #+#    #+#             */
/*   Updated: 2025/03/11 10:58:01 by rcorlett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

//---------- LIBRARIES ----------

# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>

//---------- MACROS ----------

enum e_forks
{
	LEFT,
	RIGHT,
};

enum e_actions
{
	DEATH,
	EATING,
	SLEEPING,
	THINKING,
	FORK,
	FULL
};

//---------- STRUCTURES ----------

struct	s_data;

typedef struct s_philo
{
	struct s_data	*data;
	u_int64_t		id;
	u_int64_t		meals;
	u_int64_t		t_death;
	int				status;
	bool			full;
	pthread_mutex_t	lock;
	pthread_mutex_t	*fork[2];
}					t_philo;

typedef struct s_data
{
	pthread_t		*table;
	t_philo			*philos;
	u_int64_t		n_philos;
	u_int64_t		t_death;
	u_int64_t		t_eat;
	u_int64_t		t_sleep;
	u_int64_t		n_meals;
	u_int64_t		t_start;
	u_int64_t		philos_full;
	bool			finish;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	pthread_mutex_t	log;
	pthread_mutex_t	finish_lock;
}					t_data;

//---------- FUNCTIONS ----------

//activities
void		eating(t_philo *philo);
void		sleeping(t_philo *philo);
void		thinking(t_philo *philo);

//checkers
int			check_input(char **av);
bool		is_solo(t_philo *philo);
bool		is_dead(t_philo *philo);

//routines
void		*routine(void *philo_ptr);
void		logs(t_philo *p, int status);

//utils
u_int64_t	get_time(void);
void		wait_time(t_philo *philo, u_int64_t time);
long		ft_atol(const char *nptr);
void		free_data(t_data *data);

//init
t_data		*init_data(int ac, char **av);

#endif

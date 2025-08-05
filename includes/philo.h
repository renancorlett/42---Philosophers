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
# include <pthread.h> //Library for Threads and mutex
# include <sys/time.h> //Time from the system
# include <sys/types.h> //Specific times from the system
# include <stdlib.h> //memory allocation
# include <unistd.h> //usleep, sleep and POSIX
# include <stdbool.h> //Check true or false

# define RESET  "\033[0m"
# define RED    "\033[1;31m"
# define GREEN  "\033[1;32m"

//---------- MACROS ---------- Set of instructions and commands in the source.

enum e_forks //Represents the positions of the forks
{
	LEFT,
	RIGHT,
};

enum e_actions //Control and exhibition of philos states
{
	DEATH,
	EATING,
	SLEEPING,
	THINKING,
	FORK,
	FULL
};

//---------- STRUCTURES ----------

struct	s_data; //Forward declaration - philo structure needed

typedef struct s_philo
{
	struct s_data	*data; //Pointer for globar data
	u_int64_t		id; //philo ID
	u_int64_t		meals; //number os meals eaten
	u_int64_t		t_death; //limit of time to death without eating
	int				status; //actual state
	bool			full; //Check if it is full or not, true or false
	pthread_mutex_t	lock; //Mutex for blocking the philos
	pthread_mutex_t	*fork[2]; //Pointers for the two forks (left and right)
}					t_philo;

typedef struct s_data
{
	pthread_t		*table; //Array of threads from philo
	t_philo			*philos; //Arryra of structs from philoss
	u_int64_t		n_philos; //Number of philos in the table
	u_int64_t		t_death;
	u_int64_t		t_eat;
	u_int64_t		t_sleep;
	u_int64_t		n_meals;
	u_int64_t		t_start;
	u_int64_t		philos_full;
	bool			finish; //Global flag to stop the simulation
	pthread_mutex_t	*forks; //Array of mutex from fork
	pthread_mutex_t	lock; //Mutex for global data protection
	pthread_mutex_t	log; //Mutex for protecting logs mixed with stdout
	pthread_mutex_t	finish_lock; //Mutex for protection of the finish flag
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
void		*routine(void *philo_ptr); //Main function for the philo threads
void		logs(t_philo *p, int status); //Log of philo actions

//utils
u_int64_t	get_time(void); //Time stamp in miliseconds
void		wait_time(t_philo *philo, u_int64_t time); //Active non-active waiting
long		ft_atol(const char *nptr); //Funcion to transform a string in integer
void		free_data(t_data *data); //Free memory

//init
t_data		*init_data(int ac, char **av); //Prepare the structures and init the mutexes

#endif

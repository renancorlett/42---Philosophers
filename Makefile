# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rcorlett <rcorlett@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/11 08:54:10 by rcorlett          #+#    #+#              #
#    Updated: 2025/03/11 10:12:04 by rcorlett         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
#CFLAGS = -g -Wall -Wextra -Werror -L. -lpthread -g3 -fsanitize=thread -O3 -march=native
CFLAGS = -Wall -Wextra -Werror
NAME = philo

SRC = src/main.c src/init.c src/utils.c src/routines.c \
	  src/activities.c src/checkers.c

OBJ = $(SRC:.c=.o)

all: $(NAME)
	
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -pthread $(OBJ) -o $(NAME)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

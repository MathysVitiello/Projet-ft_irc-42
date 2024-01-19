# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mvitiell <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/04 11:45:22 by mvitiell          #+#    #+#              #
#    Updated: 2024/01/19 11:04:41 by alamizan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	ircserv

SRC =	main.cpp \
		utils.cpp \
		Client/Client.cpp \
		Client/command.cpp \
		Client/mode.cpp \
		Server/Server.cpp \
		Channel/Channel.cpp \
		Channel/commandChan.cpp \

CFLAGS = -Wall -Wextra -Werror -g -std=c++98

CC = c++

OBJ = $(SRC:.cpp=.o)

%.o: %.cpp
	c++ $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY : clean fclean re all

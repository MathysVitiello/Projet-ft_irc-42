# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mvitiell <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/04 11:45:22 by mvitiell          #+#    #+#              #
#    Updated: 2024/02/29 10:23:38 by kali             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =	ircserv

SRC =	main.cpp \
		utils.cpp \
		Client/Client.cpp \
		Client/invite.cpp \
		Client/topic.cpp \
		Client/nick.cpp \
		Client/user.cpp \
		Client/pass.cpp \
		Client/kick.cpp \
		Client/join.cpp \
		Client/prvmsg.cpp \
		Client/mode.cpp \
		Client/part.cpp \
		Server/Server.cpp \
		Server/ServerMode.cpp \
		Server/allClient.cpp\
		Channel/Channel.cpp 

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

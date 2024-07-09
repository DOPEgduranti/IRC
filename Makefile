# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/03 12:26:02 by gduranti          #+#    #+#              #
#    Updated: 2024/07/09 12:18:04 by gduranti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

DEPS = includes

SRCFOLDER = src
SRC = main.cpp \
	Server.cpp \
	Client.cpp \
	utils.cpp \
	Channel.cpp

CMDFOLDER = cmd
CMD = login.cpp \
	chans.cpp \
	support.cpp
CMDS = $(addprefix $(CMDFOLDER)/, $(CMD))

SRCS = $(addprefix $(SRCFOLDER)/, $(SRC) $(CMDS))

$(NAME):
	$(CC) $(CFLAGS) $(SRCS) -I$(DEPS) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all

start: re
	./$(NAME) 4444 ciao

val: re
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$(NAME) 4444 ciao
.PHONY: all clean fclean re
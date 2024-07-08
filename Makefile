# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/03 12:26:02 by gduranti          #+#    #+#              #
#    Updated: 2024/07/08 12:38:53 by gduranti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

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

SRCS = $(addprefix $(SRCFOLDER)/, $(SRC) $(CMD))

$(NAME):
	$(CC) $(CFLAGS) $(SRCS) -I$(DEPS) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
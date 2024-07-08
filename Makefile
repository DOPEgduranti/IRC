# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/03 12:26:02 by gduranti          #+#    #+#              #
#    Updated: 2024/07/05 12:34:42 by gduranti         ###   ########.fr        #
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
	Channel.cpp \
	Server_cmds.cpp
SRCS = $(addprefix $(SRCFOLDER)/, $(SRC))

$(NAME):
	$(CC) $(CFLAGS) $(SRCS) -I$(DEPS) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
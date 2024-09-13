# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/03 12:26:02 by gduranti          #+#    #+#              #
#    Updated: 2024/09/13 10:23:17 by gduranti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g

DEPS = includes
BONUSDEPS = bonus

SRCFOLDER = src
SRC = main.cpp \
	Server.cpp \
	Client.cpp \
	utils.cpp \
	Channel.cpp

CMDFOLDER = cmd
CMD = login.cpp \
	chans.cpp \
	support.cpp \
	communication.cpp
CMDS = $(addprefix $(CMDFOLDER)/, $(CMD))

MSGFOLDER = msg
MSG = errors.cpp \
	replies.cpp
MSGS = $(addprefix $(MSGFOLDER)/, $(MSG))

BONUSFOLDER = bonus
BONUSRC = main.cpp \
		Bot.cpp
BONUSRCS = $(addprefix $(BONUSFOLDER)/, $(BONUSRC))

SRCS = $(addprefix $(SRCFOLDER)/, $(SRC) $(CMDS) $(MSGS))

$(NAME):
	$(CC) $(CFLAGS) $(SRCS) -I$(DEPS) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(NAME)
	rm -f bot

fclean: clean

re: fclean all

bonus: all
	$(CC) $(CFLAGS) $(BONUSRCS) -I$(BONUSDEPS) -o bot

start: re
	./$(NAME) 4444 ciao

robot: bonus
	./bot 10.11.5.6 4444 ciao

val: re
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$(NAME) 4444 ciao

.PHONY: all clean fclean re
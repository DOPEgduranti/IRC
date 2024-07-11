/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:10:36 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/11 16:36:06 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msg.hpp>

void RPL_WELCOME( int fd, std::string nickname ) {
	std::string message = ":server 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_YOURHOST( int fd, std::string nickname ) {
	std::string message = ":server 002 " + nickname + " :Your host is server, running version 0.1\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_CREATED( int fd, std::string nickname ) {
	std::string message = ":server 003 " + nickname + " :This server was created 2024 Jul 11\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_MYINFO( int fd, std::string nickname ) {
	std::string message = ":server 004 " + nickname + " :server 0.1 -o -o\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

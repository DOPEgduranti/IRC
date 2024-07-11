/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:24:11 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/11 16:45:54 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool Server::help( Client & cli, std::deque<std::string> input ) {
	(void) input;
	std::ifstream fin("doc/help.txt");
	if (fin.fail()) {
		std::cerr << "Server opens 'help.txt': FAILURE" << std::endl;
		return false;
	}
	std::string buffer;
	std::getline(fin, buffer, '\0');
	ft_sendMsg(cli.getFd(), buffer);
	fin.close();
	return true;
}

bool Server::ping( Client & cli, std::deque<std::string> input ) {
	input.pop_front();
	ft_sendMsg(cli.getFd(), ":server " + cli.getNickname() + " PONG :" + input.front());
	return true;
}

bool Server::pong( Client & cli, std::deque<std::string> input ) {
	(void)cli;
	(void)input;
	return true ;
}

bool Server::who( Client & cli, std::deque<std::string> input ) {
(void)cli;
	(void)input;
	return true ;
}

bool Server::userhost( Client & cli, std::deque<std::string> input ) {
	(void)cli;
	(void)input;
	return true ;
}


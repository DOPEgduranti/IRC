/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:24:11 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/03 11:54:17 by gduranti         ###   ########.fr       */
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
	if (input.size() < 2) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "PING");
		return false;
	}
	input.pop_front();
	ft_sendMsg(cli.getFd(), ":server PONG " + cli.getNickname() + " " + input.front());
	return true;
}

bool Server::pong( Client & cli, std::deque<std::string> input ) {
	(void)cli;
	(void)input;
	return true ;
}

bool Server::who( Client & cli, std::deque<std::string> input ) {
	input.pop_front();
	if (input.front() == cli.getNickname())
		ft_sendMsg(cli.getFd(), ":server 352 " + cli.getNickname() + " :<hopcount> " + cli.getRealName());
	ft_sendMsg(cli.getFd(), ":server 315 " + cli.getNickname() + " :End of /WHO list");
	return true ;
}

bool Server::userhost( Client & cli, std::deque<std::string> input ) {
	if (input.size() > 6)
		return false;
	input.pop_front();
	std::string message = ":server 302 " + cli.getNickname() + " :";
	while (!input.empty()) {
		std::vector<Client>::iterator tmp = std::find(_clients.begin(), _clients.end(), input.front());
		if (tmp != _clients.end()) {
			message += (*tmp).getNickname() + "=+" + (*tmp).getHostName();
		}
		input.pop_front();
	}
	message += "\r\n";
	send(cli.getFd(), message.c_str(), message.size(), 0);
	return true ;
}


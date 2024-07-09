/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:20:31 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/09 10:40:15 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool Server::pass( Client & cli, std::deque<std::string> input ) {
	if (input.size() > 2) {
		ft_sendMsg(cli.getFd(), "server: 'PASS' usage is PASS <key>");
		return false;
	}
	std::cout << "logged: " << cli.getLogged() << std::endl;
	if (cli.getLogged() == true) {
		ft_sendMsg(cli.getFd(), "server: You are already logged in");
		return true;
	}
	input.pop_front();
	if (input.size() != 0 && input.front() == _key) {
		cli.login();
		ft_sendMsg(cli.getFd(), "server: login SUCCESS");
		return true;
	}
	ft_sendMsg(cli.getFd(), "server: login FAILURE");
	return false;
}

bool Server::nick( Client & cli, std::deque<std::string> input ) {
	if (input.size() != 2) {
		ft_sendMsg(cli.getFd(), "server: 'NICK' usage is NICK <nickname>");
		return false;
	}
	input.pop_front();
	cli.setNickname(input.front());
	ft_sendMsg(cli.getFd(), "server: nickname change SUCCESS");
	return true;
}

bool Server::user( Client & cli, std::deque<std::string> input ) {
	if (input.size() != 5) {
		ft_sendMsg(cli.getFd(), "server: 'USER' usage is USER <username> <hostname> <servername> <realname>");
		return false;
	}
	input.pop_front();
	cli.setUser(input[0], input[1], input[2], input[3]);
	return true;
}

bool Server::quit( Client & cli, std::deque<std::string> input ) {
	(void)input;
	removeClient(cli.getFd());
	return true;
}

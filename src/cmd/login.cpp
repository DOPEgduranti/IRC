/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:20:31 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/02 10:32:04 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool Server::pass( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 2) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "PASS");
		return false;
	}
	std::cout << "logged: " << cli.getLogged() << std::endl;
	if (cli.getLogged() == true) {
		ERR_ALREADYREGISTRED(cli.getFd(), cli.getNickname());
		return true;
	}
	input.pop_front();
	if (input.size() != 0 && input.front() == _key) {
		cli.login();
		ft_sendMsg(cli.getFd(), "Server :login SUCCESS");
		return true;
	}
	ft_sendMsg(cli.getFd(), "Server :login FAILURE");
	return false;
}

bool Server::nick( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 2) {
		ERR_NONICKNAMEGIVEN(cli.getFd(), cli.getNickname());
		return false;
	}
	input.pop_front();
	if (!nickSintax(input.front())) {
		ERR_ERRONEUSNICKNAME(cli.getFd(), cli.getNickname());
			return false;
	}
	for (size_t i = 0; i < _clients.size(); i++) {
		if (input.front() == _clients[i].getNickname()) {
			ERR_NICKNAMEINUSE(cli.getFd(), cli.getNickname());
			return false;
		}
	}
	cli.setNickname(input.front());
	ft_sendMsg(cli.getFd(), "Server :nickname change SUCCESS");
	return true;
}

bool Server::user( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 5) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "USER");
		return false;
	}
	if (!cli.getUsername().empty()) {
		ERR_ALREADYREGISTRED(cli.getFd(), cli.getNickname());
		return false;
	}
	input.pop_front();
	cli.setUser(input[0], input[1], input[2], input[3]);
	RPL_WELCOME(cli.getFd(), cli.getNickname());
	RPL_YOURHOST(cli.getFd(), cli.getNickname());
	RPL_CREATED(cli.getFd(), cli.getNickname());
	RPL_MYINFO(cli.getFd(), cli.getNickname());
	return true;
}

bool Server::quit( Client & cli, std::deque<std::string> input ) {
	input.pop_front();
	if (input.empty())
		ft_sendMsg(cli.getFd(), ":server " + cli.getNickname() + " :you disconnected from server\r\n" );
	else
		ft_sendMsg(cli.getFd(), ":server " + cli.getNickname() + " :" + input.front());
	removeClient(cli);
	return true;
}

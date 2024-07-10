/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:20:31 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/10 12:19:11 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool Server::pass( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 2) {
		ft_sendMsg(cli.getFd(), "PASS" ERR_NEEDMOREPARAMS);
		return false;
	}
	std::cout << "logged: " << cli.getLogged() << std::endl;
	if (cli.getLogged() == true) {
		ft_sendMsg(cli.getFd(), "Server" ERR_ALREADYREGISTRED);
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
		ft_sendMsg(cli.getFd(), "NICK" ERR_NEEDMOREPARAMS);
		return false;
	}
	input.pop_front();
	for (size_t i = 0; i < _clients.size(); i++) {
		if (input.front() == _clients[i].getNickname()) {
			ft_sendMsg(cli.getFd(), input.front() + ERR_NICKNAMEINUSE);
			return false;
		}
	}
	cli.setNickname(input.front());
	ft_sendMsg(cli.getFd(), "Server :nickname change SUCCESS");
	return true;
}

bool Server::user( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 5) {
		ft_sendMsg(cli.getFd(), "USER" ERR_NEEDMOREPARAMS);
		return false;
	}
	if (!cli.getUsername().empty()) {
		ft_sendMsg(cli.getFd(), "Server" ERR_ALREADYREGISTRED);
		return false;
	}
	input.pop_front();
	cli.setUser(input[0], input[1], input[2], input[3]);
	ft_sendMsg(cli.getFd(), "Server :Your data has been set correctly.");
	return true;
}

bool Server::quit( Client & cli, std::deque<std::string> input ) {
	input.pop_front();
	if (input.empty())
		ft_sendMsg(cli.getFd(), "Server" MSG_QUITDEFAULT);
	else
		ft_sendMsg(cli.getFd(), "Server :" + input.front());
	removeClient(cli);
	return true;
}

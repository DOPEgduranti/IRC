/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:20:31 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/13 12:45:54 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <msg.hpp>

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
		ft_sendMsg(cli.getFd(), ":server 300 " + cli.getNickname() + " :login SUCCESS");
		return true;
	}
	ft_sendMsg(cli.getFd(), ":server 300 " + cli.getNickname() + " :login FAILURE");
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
	std::string oldNick = cli.getNickname();
	cli.setNickname(input.front());
	for (size_t i = 0; i < _clients.size(); i++)
		ft_sendMsg(_clients[i].getFd(), ":" + oldNick + "!" + cli.getUsername() + "@" + cli.getHostName() + " NICK " + cli.getNickname());
	for (size_t i = 0; i < _channels.size(); i++) {
		if (std::find(cli.getChannels().begin(), cli.getChannels().end(), _channels[i]) != cli.getChannels().end()) {
			Client tmp;
			tmp.setFd(-1);
			tmp.setNickname(cli.getNickname());
			_channels[i].broadcastMsg(tmp, "I was " + oldNick + " and now I am " + cli.getNickname());
		}
	}
	if (!cli.getUsername().empty() && !cli.getLoginMsg()) {
		RPL_WELCOME(cli.getFd(), cli.getNickname());
		RPL_YOURHOST(cli.getFd(), cli.getNickname());
		RPL_CREATED(cli.getFd(), cli.getNickname());
		RPL_MYINFO(cli.getFd(), cli.getNickname());
		cli.setLoginMsg();
	}
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
	if (!cli.getNickname().empty()) {
		RPL_WELCOME(cli.getFd(), cli.getNickname());
		RPL_YOURHOST(cli.getFd(), cli.getNickname());
		RPL_CREATED(cli.getFd(), cli.getNickname());
		RPL_MYINFO(cli.getFd(), cli.getNickname());
		cli.setLoginMsg();
	}
	return true;
}

bool Server::quit( Client & cli, std::deque<std::string> input ) {
	input.pop_front();
	std::string message = "disconnected from server";
	if (!input.empty())
		message = input.front();
	for (size_t i = 0; i < _clients.size(); i++)
		ft_sendMsg(_clients[i].getFd(), ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " QUIT " + message);
	removeClient(cli);
	return true;
}

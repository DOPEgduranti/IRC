/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:24:11 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/16 10:10:38 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <msg.hpp>

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
	if (input.size() < 2) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "WHO");
		return false;
	}
	input.pop_front();
	std::vector<Client>::iterator cliIt = std::find(_clients.begin(), _clients.end(), input.front());
	std::vector<Channel>::iterator chanIt = std::find(_channels.begin(), _channels.end(), input.front());
	if (cliIt != _clients.end())
		RPL_WHOREPLY(cli.getFd(), cli.getNickname(), "none", *cliIt);
	else if (chanIt != _channels.end())
		RPL_WHOREPLY(cli.getFd(), cli.getNickname(), *chanIt);
	else
		return false;
	RPL_ENDOFWHO(cli.getFd(), cli.getNickname(), input.front());
	return true ;
}

bool Server::userhost( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 2) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "USERHOST");
		return false;
	}
	input.pop_front();
	std::string message = ":server 302 " + cli.getNickname() + " :";
	int i = 0;
	while (!input.empty() && i < 5) {
		std::vector<Client>::iterator tmp = std::find(_clients.begin(), _clients.end(), input.front());
		if (tmp != _clients.end()) {
			message += (*tmp).getNickname() + "=-" + (*tmp).getHostName();
		}
		input.pop_front();
		if (!input.empty())
			message += " ";
		i++;
	}
	message += "\r\n";
	send(cli.getFd(), message.c_str(), message.size(), 0);
	return true ;
}

bool Server::list( Client & cli, std::deque<std::string> input ) {
	input.pop_front();
	RPL_LISTSTART(cli.getFd(), cli.getNickname());
	if (input.empty())
		for (size_t i = 0; i < _channels.size(); i++)
			RPL_LIST(cli.getFd(), cli.getNickname(), _channels[i].getName(), _channels[i].getUsers().size(), _channels[i].getTopic());
	else {
		while (!input.empty()) {
			std::vector<Channel>::iterator it = std::find(_channels.begin(), _channels.end(), input.front());
			if (it != _channels.end())
			RPL_LIST(cli.getFd(), cli.getNickname(), (*it).getName(), (*it).getUsers().size(), (*it).getTopic());
		}
	}
	RPL_LISTEND(cli.getFd(), cli.getNickname());
	return true ;
}

bool Server::whois( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 2) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "WHOIS");
		return false;
	}
	input.pop_front();
	std::deque<std::string> dq = ft_split(input.front(), ',');
	while (!dq.empty()) {
		std::vector<Client>::iterator cliIt = std::find(_clients.begin(), _clients.end(), dq.front());
		if (cliIt != _clients.end())
			RPL_WHOISUSER(cli.getFd(), cli.getNickname(), *cliIt);
		else
			ERR_NOSUCHNICK(cli.getFd(), cli.getNickname(), dq.front());
		dq.pop_front();
	}
	RPL_ENDOFWHOIS(cli.getFd(), cli.getNickname(), input.front());
	return true ;
}

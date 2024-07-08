/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:20:31 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/08 12:23:28 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool Server::pass( Client & cli, std::string str ) {
	std::cout << "logged: " << cli.getLogged() << std::endl;
	if (cli.getLogged() == true) {
		ft_sendMsg(cli.getFd(), "server: You are already logged in\n");
		return true;
	}
	size_t i = str.find("PASS") + 4;
	while (str[i] && std::isspace(str[i]))
		i++;
	if (str.substr(i, str.size() - i - 1) == _key) {
		cli.login();
		ft_sendMsg(cli.getFd(), "server: login SUCCESS\n");
		return true;
	}
	ft_sendMsg(cli.getFd(), "server: login FAILURE\n");
	return false;
}

bool Server::nick( Client & cli, std::string str ) {
	size_t i = str.find("NICK") + 4;
	while (str[i] && std::isspace(str[i]))
		i++;
	size_t j = i;
	while (str[j] && !std::isspace(str[j]))
		j++;
	if (!str[i] || str[j] != '\n') {
		ft_sendMsg(cli.getFd(), "server: NICK usage is NICK <nickname>\n");
		return false;
	}
	cli.setNickname(str.substr(i, str.size() - i - 1));
	ft_sendMsg(cli.getFd(), "server: nickname change SUCCESS\n");
	return true;
}

bool Server::user( Client & cli, std::string str ) {
	
}

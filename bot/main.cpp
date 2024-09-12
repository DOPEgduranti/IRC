/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 10:40:09 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/12 12:34:10 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

bool ipPortCheck( std::string ipAdd, std::string port ) {
	for (size_t i = 0; i < port.size(); i++) {
		if (!std::isdigit(port[i]))
			return false;
	}
	if (std::strtol(port.c_str(), NULL, 10) < 1024 || std::strtol(port.c_str(), 0, 10) > 49151)
		return false;
	if (inet_addr(ipAdd.c_str()) == INADDR_NONE)
		return false;
	return true;
}

int main ( int argc, char **argv ) {
	if (argc != 3 || ipPortCheck(argv[1], argv[2]) == false) {
		std::cerr << "Error" << std::endl;
		return 1;
	}
	Bot b(argv[1], argv[2]);
	try{
		std::cout << "---- BOT ----" << std::endl;
		signal(SIGINT, Bot::signalHandler);
		signal(SIGQUIT, Bot::signalHandler);
		b.setupBot();
	}
	catch(const std::exception& e){
		b.closeBot();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The bot Closed!" << std::endl;
	return 0;
}
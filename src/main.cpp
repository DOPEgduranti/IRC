/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:52:47 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/13 10:21:58 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

int main (int argc, char **argv) {
	if (argc != 3 || portKeyCheck(argv[1], argv[2]) == false) {
		std::cerr << "Error" << std::endl;
		return 1;
	}
	Server ser(argv[1], argv[2]);
	try{
		std::cout << "---- SERVER ----" << std::endl;
		signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
		ser.setupServer();
	}
	catch(const std::exception& e){
		ser.closePolls();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	return 0;
}
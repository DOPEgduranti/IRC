/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   communication.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 11:36:09 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/11 11:56:50 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool Server::privmsg( Client & cli, std::deque<std::string> input ) {
	(void)cli;
	(void)input;
	return true;
}

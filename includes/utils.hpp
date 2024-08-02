/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 10:51:58 by gduranti          #+#    #+#             */
/*   Updated: 2024/08/02 15:08:03 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <vector> //-> for vector
# include <sys/socket.h> //-> for socket()
# include <sys/types.h> //-> for socket()
# include <netinet/in.h> //-> for sockaddr_in
# include <fcntl.h> //-> for fcntl()
# include <unistd.h> //-> for close()
# include <arpa/inet.h> //-> for inet_ntoa()
# include <poll.h> //-> for poll()
# include <csignal> //-> for signal()
# include <cstring>
# include <algorithm>
# include <map>
# include <fstream>
# include <sstream>
# include <cstdlib>
# include <deque>
# include <msg.hpp>
# include <climits>

enum e_mode_cases {
	eErr = -1,
	eInvite,
	eTopic,
	eKey,
	eOperator,
	eLimit
};

bool portKeyCheck( std::string port, std::string key );
int ft_sendMsg( int fd, std::string msg );
std::string getFirstString( std::string & str );
std::deque<std::string> ft_split( std::string & str, char c );
std::string ft_erase( std::string & str, char c );
std::string ft_erase( std::string & str, std::string set );
bool channelSintax( std::string & str );
bool nickSintax( std::string & str );
e_mode_cases modeCases( std::string & str );
int validPositiveInteger( std::string & str );

#endif
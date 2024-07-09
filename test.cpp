#include <iostream>
#include <deque>

int main(){
	std::deque<std::string> deq;
	deq.push_back("ciao");
	deq.push_back("amico");
	deq.push_back("mio");
	std::cout << deq.front() << std::endl;
	deq.pop_front();
	std::cout << deq.front() << std::endl;
}
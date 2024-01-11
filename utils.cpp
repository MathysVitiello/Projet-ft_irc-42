#include "utils.hpp"

std::string	trimSpace(std::string str){
	for (size_t i = 0; i < str.size() && str[i] == ' '; i++){
		str.erase(i, i + 1);
		i = -1; 
	}
	std::cout << str << std::endl;
	return str;	
}

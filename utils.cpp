#include "utils.hpp"

std::string	trimSpace(std::string str){
	for (size_t i = 0; i < str.size() && str[i] == ' '; i++){
		str.erase(i, i + 1);
		i = -1; 
	}
	std::cout << str << std::endl;
	return str;	
}

void	checkArgs(int argc, char **argv)
{
	if (argc != 3)
			throw std::runtime_error( "Not the right number of arguments" ) ;

	std::string    nb = argv[1];
	for (unsigned int i = 0; i < nb.length(); i++)
		if (!isdigit (nb[i]))
			throw std::runtime_error( "the First argument is the port, pls write a number!" );
	if (std::string(argv[2]).length() == 0)
        throw std::runtime_error("write password, pls.");
}

int indexFirstArg( std::string str ){
	int i = 0;
	while (str[i] && isspace(str[i]) == 0){
		i++;
	}
	return i;
}

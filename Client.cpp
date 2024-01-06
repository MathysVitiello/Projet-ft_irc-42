#include "library.hpp"

Client::Client( void ) {

    std::cout << "Client constructor called " << std::endl;
    return ;
}

Client::~Client( void ) {

    std::cout << "Client Destructor called" << std::endl;
    return ;
}

void Client::setId( int id ) {

    this->idClient = id;
    return ;
}

void Client::setName( std::string name ) {

    this->name = name;
    return ;
}

void Client::setNick( std::string nick ) {

    this->nickname = nick;
    return ;
}

void Client::showInfo( void ) {

    std::cout << std::endl;
    std::cout << "id : " << this->idClient << std::endl;
    std::cout << "name : " << this->name << std::endl;
    std::cout << "nick : " << this->nickname << std::endl;
    std::cout << std::endl;

    return ;
}
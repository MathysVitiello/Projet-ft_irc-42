#include "Client.hpp"

void	Client::join( Server *server )
{
	//TODO si pas de mdp avec &, pas gerer
    if (_splitBuf[1].empty()){
        send(this->getSocket(), ERR_NEEDMOREPARAMS(this->getName(), "JOIN").c_str(),
            ERR_NEEDMOREPARAMS(this->getName(), "JOIN").size(), 0);
        return;
    }
	if (_splitBuf[1][0] == '#' || _splitBuf[1][0] == '&')
    {
        if (_splitBuf[1].size() <= _splitBuf[1].find(' '))
        {
            //no password
            if (_splitBuf[1][0] != '#')
                return;
            server->createChannel(*this, _splitBuf[1], "");
        }  else {
            //find password
            std::string chanel = _splitBuf[1].substr(0, _splitBuf[1].find(' '));           
		    if (chanel[0] != '&')
                return;
            _splitBuf[1] = _splitBuf[1].substr(_splitBuf[1].find(' '));
			_splitBuf[1] = trimSpace(_splitBuf[1]);

			if (_splitBuf[1][0] == '\n') {
				std::cout << "need a password" << std::endl;
				return;		
			} else if (_splitBuf[1].find(" ") < _splitBuf[1].find("\n")) {
				std::cout << "Too many arguments after the password" << std::endl;
				return;
			}
            server->createChannel(*this, chanel, _splitBuf[1]);
        }
    }
}

//tout ceux de channel doivenet recevoir un messsage, qqn c'est co
//001 au mauvais endroit

#include "Client.hpp"

void		Server::bot( Client client ){

    //pas appeler la fonction dex fois

    std::vector<Channel>::const_iterator itChan = this->getChannels().begin();
    for ( ; itChan < this->getChannels().end(); itChan++){
        if ( itChan->getName() == "~BOT" ){
            return;
        }
    }

    Channel channel( 9999, "~BOT", "" );
    this->_channels.push_back( channel );
    this->allClient(&channel, client);

    return;
}

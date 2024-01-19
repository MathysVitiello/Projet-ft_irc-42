#include "Channel.hpp"

// :Wiz TOPIC #test :New topic ; L'utilisateur Wiz définit le sujet.
// TOPIC #test :another topic ; Change le sujet du canal #test en "another topic".
// TOPIC #test ; Vérifie le sujet de #test.
void	Channel::topic( Client *client, std::string topic ){
	topic = trimSpace( topic );
	std::cout << "dsf" << std::endl;
	std::string nickname = client->getNickname();
	if ( topic.empty() ){
		if ( this->_topic == false){
			send(client->getSocket(), RPL_NOTOPIC(nickname, this->_name).c_str(),
					RPL_NOTOPIC(nickname, this->_name).size(), 0);
			return;
		}
		send(client->getSocket(), RPL_TOPIC(nickname, this->_name, this->_topicName).c_str(),
					RPL_TOPIC(nickname, this->_name, this->_topicName).size(), 0);
			return;
	}

	// if user is not in Channel 	
	std::vector<int>::iterator it = this->_ircOps.begin();
	for (; it < this->_ircOps.end(); it ++)
		if (*it == client->getSocket())
			break;
	if (it == this->_ircOps.end()){
		std::cout << "dsf" << std::endl;
		send(client->getSocket(), ERR_CHANOPRIVSNEEDED(nickname, this->_name).c_str(),
					ERR_CHANOPRIVSNEEDED(nickname, this->_name).size(), 0);
		return;
	}
	for (size_t i = 0; i < topic.size(); i++){
	       if (!isalnum(topic[i]) && topic[i] != '-' && topic[i] != '_' && topic[i] != '.' && topic[i] != '+'){
			   send (client->getSocket(), ERR_INVALIDTOPIC(nickname, this->_name).c_str(), ERR_INVALIDTOPIC(nickname, this->_name).size(), 0);
			   return ; 
		   }
	}
	this->_topic = true;
	this->_topicName = topic;
	send (client->getSocket(), RPL_TOPIC(nickname, this->_name, topic).c_str(), RPL_TOPIC(nickname, this->_name, topic).size(), 0);
}


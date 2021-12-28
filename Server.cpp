#include "Server.h"

void Server::start(ClientHandler& ch)throw(const char*){
    NewIO nio(sfd, nsfd);
    CLI cli(&nio);
}

void Server::stop(){
   // t->join(); // do not delete this!
}

Server::~Server() {
}
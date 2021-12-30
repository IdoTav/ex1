//Ido Tavron 316222512
//Tal Dabran 316040898
#include "Server.h"
#include <thread>

/*
 * this is the runStart method, which runs the cli and connect to the socket in the thread
 */
void Server::runStart() {
    while (!stopping) {
        socklen_t clilen = sizeof(cli_addr);
        alarm(9);
        int nsfd = accept(sfd, (struct sockaddr *) &cli_addr, &clilen);
        if (nsfd > 0) {
            NewIO nio(sfd, nsfd);
            CLI cli(&nio);
            cli.start();
            close(nsfd);
        }
        alarm(0);
    }
    close(sfd);
}

/*
 * this is start method which creates a new thread and run the runStart method
 */
void Server::start(ClientHandler &ch) throw(const char *) {
    //creates the thread
    t = new thread(&Server::runStart, this);
}


/*
 * this is the stop method which stop the thread
 */
void Server::stop() {
    stopping = true;
    t->join();
}

Server::~Server() {
}
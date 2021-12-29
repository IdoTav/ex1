#include "Server.h"
#include <thread>

void Server::runStart() {
    while (!stopping) {
        socklen_t clilen = sizeof(cli_addr);
        alarm(1);
        int nsfd = accept(sfd, (struct sockaddr *) &cli_addr, &clilen);
        if (nsfd > 0) {
            NewIO nio(sfd, nsfd);
            CLI cli(&nio);
            cli.start();
            close(nsfd);
        }
        alarm(0);
    }
}

void Server::start(ClientHandler &ch) throw(const char *) {
    t = new thread(&Server::runStart, this);
}

void Server::stop() {
    stopping = true;
    t->join();
}

Server::~Server() {
}
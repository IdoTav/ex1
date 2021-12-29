#ifndef SERVER_H_
#define SERVER_H_


#include <thread>
#include "CLI.h"
#include "commands.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;



class NewIO:public DefaultIO{
    int _sfd, _nsfd;
public:
    NewIO(int sfd,int nsfd){
        _sfd = sfd;
        _nsfd = nsfd;
    }
    virtual string read(){
        char c = 0;
        string s = "";
        while(c!='\n') {
            recv(_nsfd, &c,sizeof(char), 0);
            s += c;
        }
        return s;
    }
    virtual void write(string text){
        send(_nsfd, text.c_str(), text.size(), 0);
    }
    void close(){
        ::close(_nsfd);
        ::close(_sfd);
    }
    ~NewIO(){
        close();
    }
};


// edit your ClientHandler interface here:
class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){
    }
};


// implement on Server.cpp
class Server {
    int sfd;
    sockaddr_in serv_addr, cli_addr;
    thread *t;
    volatile bool stopping = false;

public:
    Server(int port) throw (const char*){
        stopping = false;
        char buffer[256];
        int n;
        sfd =  socket(AF_INET, SOCK_STREAM, 0);
        if (sfd < 0)
            cout<<("ERROR opening socket")<<endl;
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);
        if (bind(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
            cout<<("ERROR on binding")<<endl;
        listen(sfd, 5);
    }
    void stop();
    virtual ~Server();
    void start(ClientHandler& ch) throw(const char*);
    void runStart();
};

#endif /* SERVER_H_ */


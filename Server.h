//Ido Tavron 316222512
//Tal Dabran 316040898
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

    /*
     * this is the read function which read line by line from the file
     */
    virtual string read(){
        // define a char
        char c = 0;
        // and empty string
        string s = "";
        // scan until we get to '\n'
        while(c!='\n') {
            // get the c
            recv(_nsfd, &c,sizeof(char), 0);
            // concat c to s
            s += c;
        }
        // return s
        return s;
    }

    /*
     * this is the write function that sent the string
     * we are want to write in the file
     */
    virtual void write(string text){
        send(_nsfd, text.c_str(), text.size(), 0);
    }
    ~NewIO(){
    }
};


class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};


class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){
    }
};

class Server {
    int sfd;
    sockaddr_in serv_addr, cli_addr;
    thread *t;
    volatile bool stopping = false;

public:
    /*
     * this is the constructor of the server class
     * which creates the server socket
     */
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


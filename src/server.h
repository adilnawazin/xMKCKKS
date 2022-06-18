#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <NTL/ZZ.h>


using namespace std;
using namespace NTL;


class server
{
    public:
        server();
        virtual ~server();
        static int estab_conn(int port);
        static int tcp_recv(int clientsocket, char* buffer);
        static int tcp_send(int client_socket, char* buffer);

    protected:

    private:
};

#endif // SERVER_H
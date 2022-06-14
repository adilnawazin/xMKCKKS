#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string>
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

class client
{
    public:
        client();
        //static int create_sock();
        static int connect_serv (int port, string ipadd);
        static void client_send(int sock_connected, char* stream);
        static ZZ* client_receive(int sock_connected, char* buffer);
        virtual ~client();

    protected:

    private:
};

#endif // CLIENT_H

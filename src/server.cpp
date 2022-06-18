#include "server.h"
using namespace std;
using namespace NTL;

server::server()
{
    //ctor
}

server::~server()
{
    //dtor
}

int server::estab_conn(int port)
{
    int listening  = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
        cerr << "can't bind to address and port" << endl;
    }
    listen(listening, SOMAXCONN);
    sockaddr_in client;
    socklen_t clientsize = sizeof(client);
    int clientsocket = accept(listening, (sockaddr*)&client, &clientsize);
    if (clientsocket == -1)
    {
        cerr << "client could not connect" << endl;
    }
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port " << svc << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }
    close(listening);
    return clientsocket;
}

int server::tcp_recv(int clientsocket, char* buffer)
{


        int bytesrcvd = recv(clientsocket, buffer, 512, MSG_WAITALL);
        if (bytesrcvd == -1)
        {
            server server;
            server.tcp_recv(clientsocket, buffer);
        }
        if (bytesrcvd == 0)
        {
            cout << "Client has closed the connection" << endl;
        }
        return bytesrcvd;
}
int server::tcp_send(int client_socket, char* buffer)
{
    // ostringstream stream.str().c_str());
	int bytesend = send(client_socket, buffer ,512, MSG_WAITALL);
    return bytesend;
}
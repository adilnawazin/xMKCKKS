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

int server::estab_conn()
{
    int listening  = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
        cerr << "can't bind to address and port" << endl;
        return errno;
    }
    listen(listening, SOMAXCONN);
    sockaddr_in client;
    socklen_t clientsize = sizeof(client);
    int clientsocket = accept(listening, (sockaddr*)&client, &clientsize);
    if (clientsocket == -1)
    {
        cerr << "client could not connect" << endl;
        return errno;
    }
    close(listening);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);
    return clientsocket;
}

void server::tcp_recv(int clientsocket, char* buffer)
{
    while(1)
    {

        memset(buffer , 0 , 4096);
        int bytesrcvd = recv(clientsocket, buffer, 4096, 0);
        if (bytesrcvd == -1)
        {
            cerr << "Connection Error : Exiting";
            break;
        }
        if (bytesrcvd == 0)
        {
            cout << "Client has closed the connection" << endl;
            break;
        }
        string str(buffer);
        ZZ* x = new ZZ;
        *x = conv<ZZ>(buffer);
        cout << sizeof(x) << endl;
        cout << *x << endl;
    }
}
void server::tcp_send(int client_socket, char* buffer)
{
    // ostringstream msgstream;
	//msgstream << *message;
	//char buffer[sizeof(stream)+1];	//as 1 char space for null is also required
	//strcpy(buffer, stream.str().c_str());
	int bytesend = send(client_socket, buffer , 4096, 0);

	if (bytesend == -1)
	{
		cout << "Could not send to server! Whoops!\r\n";
		return;
	}
}
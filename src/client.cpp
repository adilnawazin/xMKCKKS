#include "client.h"
#include <NTL/ZZ.h>
using namespace std;
using namespace NTL;

client::client()
{
    //ctor
}
client::~client()
{
    //dtor
}

int client::connect_serv (int port, string ipadd)
{
	int sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock == -1)
	{
		cerr << "Socket not created" << endl;
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipadd.c_str(), &hint.sin_addr);
    //	Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    while(connectRes==-1)
	{
		cout<< "couldn't connect to server Retrying! \t\r"<<flush;
		connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
	}
	return sock;
}

int client::client_send(int sock_connected, char* stream)
{
	int bytesend = send(sock_connected, stream , 512,MSG_WAITALL);
	if (bytesend == -1)
        {
		cout << "Could not send to server! Whoops! Retrying\r\n";
        }
	return bytesend;
}

int client::client_receive(int sock_connected, char* buffer)
{

	int bytesrcvd = recv(sock_connected, buffer, 512, MSG_WAITALL);
	if(bytesrcvd == 0)
	{
		cout << "Connection lost"<<endl;
	}
	if(bytesrcvd == -1)
	{
		cerr << "error in receive" << endl;
	}
	return bytesrcvd;
}
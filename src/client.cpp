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
    if (connectRes == -1)
    {
        cerr << "can't connect to server" << endl;
    }
	return sock;
}

void client::client_send(int sock_connected, char* stream)
{
	int bytesend = send(sock_connected, stream , 4096, 0);

	if (bytesend == -1)
	{
		cout << "Could not send to server! Whoops!\r\n";
		return;
	}
    close(sock_connected);
}

ZZ* client::client_receive(int sock_connected, char* buffer)
{

	int bytesrcvd = recv(sock_connected, buffer, 4096, 0);
	if(bytesrcvd == -1)
	{
		cerr << "error in receive" << endl;
	}

	string str(buffer);
	ZZ* ax = new ZZ;
	*ax = conv<ZZ>(buffer);
    cout << "ax = " << *ax << endl;
	//close(sock_connected);
	close(sock_connected);
	return ax;
}
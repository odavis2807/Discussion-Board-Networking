#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define INVALID_SOCKET 0
#define SOCKET_ERROR -1
using namespace std;
void input(string author, string topic, string message);

int main() {
string author;
string topic;
string message;
input(author, topic, message);

}

int sendMessage(string finalMessage){
//create the client socket
int ClientSocket;
ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//if the socket fails print a message and terminate the windows sockets
if (ClientSocket == INVALID_SOCKET) {
cout << "ERROR: Failed to create ServerSocket" << std::endl;
return 0;
}

sockaddr_in SvrAddr;
SvrAddr.sin_family = AF_INET;
//set the port number
SvrAddr.sin_port = htons(27500);
SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//if the connection fails terminate the sockets and print an error
if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
close(ClientSocket);

cout << "ERROR: Connection attempted failed" << std::endl;
return 0;
}

    //send the message, and store the result
int result = send(ClientSocket, finalMessage.c_str(), finalMessage.length(), 0);
//if the send fails print an error
if (result != finalMessage.length())
cout << "Error on send";
else
{
char response[2];
int bytesRead = recv(ClientSocket, response, 2, 0);
if (bytesRead == -1)
{
cout << "Problem with transmission";
}
else if (response[0] == '1')
cout << "Success";
else
cout << "Failure";
}
//close and terminate the socket
close(ClientSocket);
return 1;
}


void input(string author, string topic, string message){
    string finalMessage = author + " | " + topic + " | " + message + " | " + "<EOM>";
    sendMessage(finalMessage);
}


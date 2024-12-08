#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#define INVALID_SOCKET 0
#define SOCKET_ERROR -1
using namespace std;

typedef struct Message{
string author;
string topic;
string message;
};

vector<string> split(const string& s, const string& delimiter) ;
void input(Message m);

int ClientSocket;

int main() {
//Message m;
//input(m);


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

    //send the message, and store the result
int result = send(ClientSocket, finalMessage.c_str(), finalMessage.length(), 0);
//if the send fails print an error
if (result != finalMessage.length())
    return 0;

return 1;
}

void openConnection(){
ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//if the socket fails print a message and terminate the windows sockets
if (ClientSocket == INVALID_SOCKET) {
cout << "ERROR: Failed to create ServerSocket" << std::endl;
return;
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
}
}


void closeConnection(){
//close and terminate the socket
close(ClientSocket);
}

void recieve(Message &m){

char response[1000];
int bytesRead = recv(ClientSocket, response, 2, 0);
if (bytesRead == -1)
{
cout << "Problem with transmission";
}
else {
response[bytesRead] = 0;
string str(response);
vector <string> v = split(str, " | ");
m.author = v[1];
m.topic = v[2];
m.message = v[3];
}

}

void input(Message m){
    string finalMessage = m.author + " | " + m.topic + " | " + m.message + " | " + "<EOM>";
    sendMessage(finalMessage);
}

vector<string> split(const string& s, const string& delimiter) {
    vector<string> tokens;
    size_t pos = 0;
    string s2 = s;
    string token;
    while ((pos = s2.find(delimiter)) != string::npos) {
        token = s2.substr(0, pos);
        tokens.push_back(token);
        s2 = s2.substr(pos, delimiter.length() - pos);
        //s.erase((size_t)0, (size_t)(pos + delimiter.length()));
    }
    tokens.push_back(s);

    return tokens;
}
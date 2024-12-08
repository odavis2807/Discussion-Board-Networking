#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <vector>
#include <sstream>

#define INVALID_SOCKET 0
#define SOCKET_ERROR -1
using namespace std;

struct Post {
    string author;
    string topic;
    string message;
};

// Function to parse the message and extract post details
Post parseMessage(const string& message) {
    Post post;
    stringstream ss(message);
    getline(ss, post.author, '|');
    getline(ss, post.topic, '|');
    getline(ss, post.message, '|');
    return post;
}

// Function to save posts to a file
void savePostToFile(const Post& post, const string& filename) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << post.author << "|" << post.topic << "|" << post.message << endl;
        file.close();
    } else {
        cerr << "ERROR: Could not open file to save post." << endl;
    }
}

// Main server function
int main() {
    // Create server socket
    int ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ServerSocket == INVALID_SOCKET) {
        cerr << "ERROR: Failed to create ServerSocket" << endl;
        return -1;
    }

    // Bind the socket to an IP address and port
    sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(27500);
    SvrAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(ServerSocket, (struct sockaddr)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR) {
        cerr << "ERROR: Failed to bind ServerSocket" << endl;
        close(ServerSocket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(ServerSocket, 1) == SOCKET_ERROR) {
        cerr << "ERROR: Failed to listen on ServerSocket" << endl;
        close(ServerSocket);
        return -1;
    }

    cout << "Server is running. Waiting for connections..." << endl;

    while (true) {
        // Accept a client connection
        sockaddr_in ClientAddr;
        socklen_t AddrLen = sizeof(ClientAddr);
        int ClientSocket = accept(ServerSocket, (struct sockaddr)&ClientAddr, &AddrLen);

        if (ClientSocket == INVALID_SOCKET) {
            cerr << "ERROR: Failed to accept client connection" << endl;
            continue;
        }

        cout << "Client connected." << endl;

        // Receive message from the client
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(ClientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesRead <= 0) {
            cerr << "ERROR: Failed to receive message from client" << endl;
            close(ClientSocket);
            continue;
        }

        string receivedMessage(buffer);
        cout << "Received: " << receivedMessage << endl;

        // Process the message
        if (receivedMessage.find("<EOM>") != string::npos) {
            Post post = parseMessage(receivedMessage);
            savePostToFile(post, "posts.txt");
            cout << "Post saved: " << post.author << " | " << post.topic << " | " << post.message << endl;

            // Respond to the client
            const char* successResponse = "1";
            send(ClientSocket, successResponse, strlen(successResponse), 0);
        } else {
            const char* failureResponse = "0";
            send(ClientSocket, failureResponse, strlen(failureResponse), 0);
        }

        // Close the client connection
        close(ClientSocket);
        cout << "Client disconnected." << endl;
    }

    // Close the server socket
    close(ServerSocket);
    return 0;
}

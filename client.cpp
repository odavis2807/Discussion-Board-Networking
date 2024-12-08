#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
using namespace std;

// Message structure
struct Message {
    string author;
    string topic;
    string message;
};

// Function prototypes
vector<string> split(const string& s, const string& delimiter);
void openConnection();
void closeConnection();
void receiveLogs();
void sendMessage(const Message& m);

// Global client socket
int ClientSocket;

int main() {
    // Open connection to the server
    openConnection();

    // Step 1: Receive and display all previous logs
    receiveLogs();

    // Step 2: Input and send a new message
    char choice;
    do {
        Message m;
        cout << "\nEnter author: ";
        getline(cin, m.author);
        cout << "Enter topic: ";
        getline(cin, m.topic);
        cout << "Enter message: ";
        getline(cin, m.message);

        sendMessage(m);

        cout << "Do you want to send another message? (y/n): ";
        cin >> choice;
        cin.ignore(); // Clear the input buffer
    } while (tolower(choice) == 'y');

    // Close connection to the server
    closeConnection();

    return 0;
}

// Function to open a connection to the server
void openConnection() {
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET) {
        cerr << "ERROR: Failed to create client socket." << endl;
        exit(1);
    }

    sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(27500); // Server port
    SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP

    // Connect to the server
    if (connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR) {
        cerr << "ERROR: Failed to connect to server." << endl;
        close(ClientSocket);
        exit(1);
    }

    cout << "Connected to server successfully." << endl;
}

// Function to close the connection to the server
void closeConnection() {
    close(ClientSocket);
    cout << "Connection to server closed." << endl;
}

// Function to receive and display logs from the server
void receiveLogs() {
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(ClientSocket, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead > 0) {
        cout << "\n--- Previous Logs from Server ---\n";
        cout << buffer << endl;
        cout << "----------------------------------\n";
    } else {
        cerr << "ERROR: Failed to receive logs from server or no logs available." << endl;
    }
}

// Function to send a new message to the server
void sendMessage(const Message& m) {
    // Format the message
    string finalMessage = m.author + " | " + m.topic + " | " + m.message + " | <EOM>";

    // Send the message to the server
    int result = send(ClientSocket, finalMessage.c_str(), finalMessage.length(), 0);
    if (result == SOCKET_ERROR) {
        cerr << "ERROR: Failed to send message to server." << endl;
        return;
    }

    // Receive the server's response
    char response[2];
    int bytesRead = recv(ClientSocket, response, sizeof(response) - 1, 0);
    if (bytesRead > 0 && response[0] == '1') {
        cout << "Server response: Message successfully saved." << endl;
    } else {
        cout << "Server response: Failed to process the message." << endl;
    }
}

// Utility function to split a string into tokens
vector<string> split(const string& s, const string& delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));
    return tokens;
}

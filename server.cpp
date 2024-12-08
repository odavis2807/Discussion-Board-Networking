#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

struct Message {
    string author;
    string topic;
    string message;
};

vector<Message> loadLogs(const string& filename);
string formatLogs(const vector<Message>& logs);
void saveLog(const Message& msg, const string& filename);
Message parseMessage(const string& rawMessage);

int main() {
    const string filename = "logs.txt";
    vector<Message> logs = loadLogs(filename);

    // Create server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1) {
        cerr << "ERROR: Failed to create server socket" << endl;
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(27500);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "ERROR: Failed to bind server socket" << endl;
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, 1) == -1) {
        cerr << "ERROR: Failed to listen on server socket" << endl;
        close(serverSocket);
        return -1;
    }

    cout << "Server is running and waiting for connections..." << endl;

    while (true) {
        sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);

        if (clientSocket == -1) {
            cerr << "ERROR: Failed to accept client connection" << endl;
            continue;
        }

        cout << "Client connected." << endl;

        // Step 1: Send all previous logs to the client
        string formattedLogs = formatLogs(logs);
        send(clientSocket, formattedLogs.c_str(), formattedLogs.length(), 0);
        cout << "Previous logs sent to client." << endl;

        // Step 2: Receive and process multiple messages
        char buffer[1024];
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

            if (bytesRead <= 0) {
                cout << "Client disconnected." << endl;
                break; // Exit the loop if the client disconnects
            }

            cout << "Received message from client: " << buffer << endl;

            // Parse the message and save it to logs
            string receivedMessage(buffer);
            Message newMessage = parseMessage(receivedMessage);
            logs.push_back(newMessage);
            saveLog(newMessage, filename);

            cout << "New message saved: " << newMessage.author << " | " 
                 << newMessage.topic << " | " << newMessage.message << endl;

            // Send success response
            send(clientSocket, "1", 1, 0);
        }

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}

// Function to load logs from a file
vector<Message> loadLogs(const string& filename) {
    vector<Message> logs;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            Message msg;
            getline(ss, msg.author, '|');
            getline(ss, msg.topic, '|');
            getline(ss, msg.message, '|');
            logs.push_back(msg);
        }
        file.close();
    } else {
        cout << "No existing logs found." << endl;
    }
    return logs;
}

// Function to format logs for transmission to the client
string formatLogs(const vector<Message>& logs) {
    stringstream formattedLogs;
    for (const auto& log : logs) {
        formattedLogs << log.author << " | " << log.topic << " | " 
                      << log.message << " | <EOM>\n";
    }
    return formattedLogs.str();
}

// Function to save a log entry to a file
void saveLog(const Message& msg, const string& filename) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << msg.author << "|" << msg.topic << "|" << msg.message << endl;
        file.close();
    } else {
        cerr << "ERROR: Could not open file to save log." << endl;
    }
}

// Function to parse a raw message into a Message struct
Message parseMessage(const string& rawMessage) {
    Message msg;
    stringstream ss(rawMessage);
    getline(ss, msg.author, '|');
    getline(ss, msg.topic, '|');
    getline(ss, msg.message, '|');
    return msg;
}


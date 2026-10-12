Program Code : 

Server.cpp
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    sockaddr_in server, client;
    int clientSize = sizeof(client);
    char buffer[1024];

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create TCP socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Bind socket
    bind(serverSocket, (sockaddr*)&server, sizeof(server));

    // Listen for clients
    listen(serverSocket, 1);

    cout << "Server is waiting for client..." << endl;

    // Accept client
    clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);

    cout << "Client connected!" << endl;

    // Receive message
    int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    buffer[bytes] = '\0';

    cout << "Message from client: " << buffer << endl;

    // Send reply
    string reply = "Hello from Server!";
    send(clientSocket, reply.c_str(), reply.length(), 0);

    // Close connections
    closesocket(clientSocket);
    closesocket(serverSocket);

    WSACleanup();

    return 0;
}

Client.cpp
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsa;
    SOCKET clientSocket;
    sockaddr_in server;
    char buffer[1024];

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create TCP socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8080);

    // Connect to server
    connect(clientSocket, (sockaddr*)&server, sizeof(server));

    // Input message
    string message;

    cout << "Enter message: ";
    getline(cin, message);

    // Send message
    send(clientSocket, message.c_str(), message.length(), 0);

    // Receive reply
    int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    buffer[bytes] = '\0';

    cout << "Reply from server: " << buffer << endl;

    // Close socket
    closesocket(clientSocket);

    WSACleanup();

    return 0;
}

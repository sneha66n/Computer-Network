Program Code:
Udp_server.cpp
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsa;
    SOCKET serverSocket;
    sockaddr_in server, client;
    int clientSize = sizeof(client);
    char buffer[1024];

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create UDP socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    // Bind socket
    bind(serverSocket, (sockaddr*)&server, sizeof(server));

    cout << "UDP Server is waiting for message..." << endl;

    // Receive message
    int bytes = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0,
                         (sockaddr*)&client, &clientSize);

    buffer[bytes] = '\0';

    cout << "Message from client: " << buffer << endl;

    // Send reply
    string reply = "Hello from UDP Server!";
    sendto(serverSocket, reply.c_str(), reply.length(), 0,
           (sockaddr*)&client, clientSize);

    cout << "Reply sent to client." << endl;

    // Close socket
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}

Udp_client.cpp
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsa;
    SOCKET clientSocket;
    sockaddr_in server;
    int serverSize = sizeof(server);
    char buffer[1024];

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create UDP socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8080);

    // Input message
    string message;

    cout << "Enter message: ";
    getline(cin, message);

    // Send message to server
    sendto(clientSocket, message.c_str(), message.length(), 0,
           (sockaddr*)&server, sizeof(server));

    // Receive reply
    int bytes = recvfrom(clientSocket, buffer, sizeof(buffer) - 1, 0,
                         (sockaddr*)&server, &serverSize);

    buffer[bytes] = '\0';

    cout << "Reply from server: " << buffer << endl;

    // Close socket
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}

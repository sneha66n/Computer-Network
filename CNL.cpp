//Hamming
#include <iostream>
using namespace std;

int main() {
    string bits;
    cout << "Enter 4 data bits (example 1011): ";
    cin >> bits;

    int d1 = bits[0] - '0';
    int d2 = bits[1] - '0';
    int d3 = bits[2] - '0';
    int d4 = bits[3] - '0';

    // parity bits
    int p1 = d1 ^ d2 ^ d4;
    int p2 = d1 ^ d3 ^ d4;
    int p3 = d2 ^ d3 ^ d4;

    // code order: p1 p2 d1 p3 d2 d3 d4
    int code[7] = {p1, p2, d1, p3, d2, d3, d4};

    cout << "Sent code    : ";
    for (int i = 0; i < 7; i++) cout << code[i];
    cout << endl;

    int pos;
    cout << "Position to make wrong (1-7, 0 for none): ";
    cin >> pos;
    if (pos != 0) code[pos - 1] = 1 - code[pos - 1];   // flip the bit

    cout << "Received code: ";
    for (int i = 0; i < 7; i++) cout << code[i];
    cout << endl;

    // receiver checks
    int c1 = code[0] ^ code[2] ^ code[4] ^ code[6];
    int c2 = code[1] ^ code[2] ^ code[5] ^ code[6];
    int c3 = code[3] ^ code[4] ^ code[5] ^ code[6];
    int error = c3 * 4 + c2 * 2 + c1;

    if (error == 0) {
        cout << "No error" << endl;
    } else {
        cout << "Error at position " << error << endl;
        code[error - 1] = 1 - code[error - 1];          // correct it
        cout << "Corrected code: ";
        for (int i = 0; i < 7; i++) cout << code[i];
        cout << endl;
    }

    cout << "Data bits    : " << code[2] << code[4] << code[5] << code[6] << endl;
    return 0;
}


//Subnet
#include <iostream>
using namespace std;

int main() {
    int prefix = 24;      // 192.168.1.0/24
    int subnets = 2;      // how many subnets we want

    // how many bits to borrow?
    int bits = 0;
    int power = 1;
    while (power < subnets) {
        power = power * 2;
        bits = bits + 1;
    }

    int newPrefix = prefix + bits;
    int hostBits = 32 - newPrefix;

    int total = 1;                       // addresses in each subnet
    for (int i = 0; i < hostBits; i++) total = total * 2;

    int usable = total - 2;              // remove network and broadcast
    int mask = 256 - total;              // last number of the subnet mask

    cout << "New prefix: /" << newPrefix << endl;
    cout << "Subnet mask: 255.255.255." << mask << endl;
    cout << "Usable hosts in each subnet: " << usable << endl << endl;

    for (int i = 0; i < subnets; i++) {
        int start = i * total;
        cout << "Subnet " << i + 1 << endl;
        cout << "  Network   : 192.168.1." << start << endl;
        cout << "  First host: 192.168.1." << start + 1 << endl;
        cout << "  Last host : 192.168.1." << start + total - 2 << endl;
        cout << "  Broadcast : 192.168.1." << start + total - 1 << endl;
    }
    return 0;
}


//DV
#include <iostream>
using namespace std;

int main() {
    string names[3] = {"R1", "R2", "R3"};

    // cost between routers (use 999 if there is no direct link)
    int cost[3][3] = {{0, 2, 7},
                      {2, 0, 3},
                      {7, 3, 0}};

    // each router starts knowing only its direct links
    int dist[3][3] = {{0, 2, 7},
                      {2, 0, 3},
                      {7, 3, 0}};

    // next router to use for each destination
    int nxt[3][3] = {{0, 1, 2},
                     {0, 1, 2},
                     {0, 1, 2}};

    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < 3; i++)            // router i
            for (int j = 0; j < 3; j++)        // destination j
                for (int k = 0; k < 3; k++) {  // go through neighbour k
                    int newCost = cost[i][k] + dist[k][j];
                    if (newCost < dist[i][j]) {
                        dist[i][j] = newCost;
                        nxt[i][j] = k;
                        changed = true;
                    }
                }
    }

    for (int i = 0; i < 3; i++) {
        cout << "Routing table of " << names[i] << endl;
        for (int j = 0; j < 3; j++)
            cout << "  to " << names[j] << " cost " << dist[i][j]
                 << " next hop " << names[nxt[i][j]] << endl;
        cout << endl;
    }

    // shortest path from R1 (0) to R3 (2)
    int start = 0, end = 2;
    string path = names[start];
    int now = start;
    while (now != end) {
        now = nxt[now][end];
        path = path + " -> " + names[now];
    }
    cout << "Shortest path R1 to R3: " << path << " | cost = " << dist[start][end] << endl;
    return 0;
}




//TCP
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);         // TCP socket
    int yes = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);                     // port
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");   // IP

    bind(s, (sockaddr*)&addr, sizeof(addr));
    listen(s, 1);
    cout << "Server waiting..." << endl;

    int conn = accept(s, NULL, NULL);                // wait for client
    cout << "Client connected" << endl;

    char message[1024] = {0};
    recv(conn, message, 1023, 0);                    // receive message
    cout << "Message received: " << message << endl;

    close(conn);
    close(s);
    return 0;
}

//client
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int main() {
    int c = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(c, (sockaddr*)&addr, sizeof(addr));      // connect to server

    char message[1024];
    cout << "Enter message: ";
    cin.getline(message, 1024);
    send(c, message, strlen(message), 0);            // send message

    close(c);
    return 0;
}



//calculation
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(s, (sockaddr*)&addr, sizeof(addr));
    listen(s, 1);
    cout << "Calculator server ready..." << endl;

    int conn = accept(s, NULL, NULL);

    char data[1024] = {0};
    recv(conn, data, 1023, 0);                       // example: "12 4 /"

    double a, b, result = 0;
    char op;
    sscanf(data, "%lf %lf %c", &a, &b, &op);         // split into a, b, op

    char reply[100];
    if (op == '+') {
        result = a + b;
        sprintf(reply, "%g", result);
    } else if (op == '-') {
        result = a - b;
        sprintf(reply, "%g", result);
    } else if (op == '*') {
        result = a * b;
        sprintf(reply, "%g", result);
    } else if (op == '/') {
        if (b == 0) {
            strcpy(reply, "Cannot divide by zero");
        } else {
            result = a / b;
            sprintf(reply, "%g", result);
        }
    } else {
        strcpy(reply, "Wrong operator");
    }

    cout << "Received: " << data << " -> Result: " << reply << endl;
    send(conn, reply, strlen(reply), 0);             // send result back

    close(conn);
    close(s);
    return 0;
}

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int main() {
    int c = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(c, (sockaddr*)&addr, sizeof(addr));

    string a, b, op;
    cout << "Enter first number: ";
    cin >> a;
    cout << "Enter second number: ";
    cin >> b;
    cout << "Enter operator (+ - * /): ";
    cin >> op;

    string message = a + " " + b + " " + op;
    send(c, message.c_str(), message.length(), 0);   // send all three

    char result[1024] = {0};
    recv(c, result, 1023, 0);                        // get the answer
    cout << "Result from server: " << result << endl;

    close(c);
    return 0;
}

//UDP
#include <iostream>
#include <cstring>
#include <cctype>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0);          // UDP socket

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(s, (sockaddr*)&addr, sizeof(addr));
    cout << "UDP server ready..." << endl;

    char text[1024] = {0};
    sockaddr_in client = {};
    socklen_t size = sizeof(client);
    recvfrom(s, text, 1023, 0, (sockaddr*)&client, &size);   // get text and who sent it
    cout << "Received: " << text << endl;

    for (int i = 0; text[i] != '\0'; i++)
        text[i] = toupper(text[i]);                  // make capital letters

    sendto(s, text, strlen(text), 0, (sockaddr*)&client, size);  // send back
    close(s);
    return 0;
}

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int main() {
    int c = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server = {};
    server.sin_family = AF_INET;
    server.sin_port = htons(7000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    char text[1024];
    cout << "Enter a string: ";
    cin.getline(text, 1024);
    sendto(c, text, strlen(text), 0, (sockaddr*)&server, sizeof(server));   // send

    char result[1024] = {0};
    recvfrom(c, result, 1023, 0, NULL, NULL);        // get the reply
    cout << "Converted string: " << result << endl;

    close(c);
    return 0;
}

//
//  server.cpp
//  TCPServer
//
//  Created by Henrique Valcanaia on 2019-11-24.
//  Copyright © 2019 Henrique Valcanaia. All rights reserved.
//

#include "../include/server.h"

using namespace std;

int serverSocket;
ofstream outfile;

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        cout << "Missing port number!" << endl;
        cout << "Usage:" << endl;
        cout << "   server <port number>" << endl;
        return -1;
    }

    string c_port(argv[1]);
    string filepath = "../log/srv_" + c_port + ".csv";
    outfile.open(filepath);
    outfile << "index,bandwidth,time" << endl;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cout << "Error (" << errno << ") trying to create socket" << endl;
        exit(EXIT_FAILURE);
    }

    // Setup socket
    // The socket address of the connecting client that is filled in by accept() before it returns.
    // The format of address is determined by the domain that the client resides in. 
    // This parameter can be NULL if the caller is not interested in the client address.
    struct sockaddr_in address;
    int port = atoi(argv[1]);
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind socket
    int addrlen = sizeof(address);
    if (bind(serverSocket, (struct sockaddr *)&address, addrlen) < 0) {
        cout << "Error (" << errno << ") trying to bind" << endl;
        exit(EXIT_FAILURE);
    }

    // Prepare the server for incoming client requests
    int pedingConnectionsQueueSize = 3;
    if (listen(serverSocket, pedingConnectionsQueueSize) < 0) {
        cout << "Error " << errno << " trying to listen" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Server listening on port " << port << "..." << endl;

    // Accept a new connection on a socket
    serverSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (serverSocket < 0) {
        cout << "Error " << errno << " trying to accept" << endl;
        exit(EXIT_FAILURE);
    }

    // Start logging and trigger read data
    std::thread t1(logSpeed);
    readData();

    cout << "Hit <return> to stop" << endl;
    getchar();

    close(serverSocket);
    outfile.close();

    cout << "Closed server on port " << port << endl;

    return 0;
}

double totalBytesRead = 0;
void readData() {
    char buffer[PKT_SIZE] = {0};
    int bytesRead = 0;
    while (1) {
        bytesRead = read(serverSocket, buffer, PKT_SIZE);
        if (bytesRead > 0) {
            totalBytesRead += bytesRead;
        }
    }
}

double totalBytesReadASecondAgo = 0;
void logSpeed(){
    int logIndex = 0;
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        double bandwidth = (totalBytesRead-totalBytesReadASecondAgo)/1024/1024*8;
        totalBytesReadASecondAgo = totalBytesRead;
        std::time_t t = std::time(0);
        cout << bandwidth << " Mbit/s" << endl;
        outfile << logIndex++ << "," << bandwidth << "," << t << endl;
    }
}
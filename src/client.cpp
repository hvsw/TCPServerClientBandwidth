//
//  client.cpp
//  TCPClient
//
//  Created by Henrique Valcanaia on 2019-11-24.
//  Copyright © 2019 Henrique Valcanaia. All rights reserved.
//

#include "../include/client.h"

int sock, valread;
ofstream outfile;

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        cout << "Missing port number!" << endl;
        cout << "Usage:" << endl;
        cout << "   client <port number>" << endl;
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    string c_port(argv[1]);
    string filepath = "../log/cli_" + c_port + ".csv";
    outfile.open(filepath);
    outfile << "index, bandwidth, time" << endl;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cout << "Socket creation error ("<< errno << ")" << endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // This can be AF_INET or AF_INET6
    int addressFamily = AF_INET;

    // Buffer holding an IPv4 address if the address family is AF_INET,
    // or an IPv6 address if the address family is AF_INET6
    const char *src = IP_ADDR;

    // Buffer to store the resulting text string; it must not be NULL
    void *dst = &serv_addr.sin_addr;
    if(inet_pton(addressFamily, src, dst) <= 0) {
        cout << "Address invalid or not supported (" << errno << ")" << endl;
        exit(EXIT_FAILURE);
    }

    // http://man7.org/linux/man-pages/man2/connect.2.html
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Unable to connect (" << errno << ")" << endl;
        exit(EXIT_FAILURE);
    }

    std::thread t1(logSpeed);
    std::thread t2(sendData);
    cout << "Hit <return> to stop" << endl;
    getchar();

    close(sock);
    outfile.close();

    return 0;
}

double totalBytesSent = 0;
char data[PKT_SIZE];
void sendData() {
    int bytesSent = 0;
    while (1) {
        bytesSent = send(sock, data, PKT_SIZE, 0);
        if (bytesSent > 0) {
          totalBytesSent += bytesSent;
        } else {
          // We won't treat any errors for now
        }
    }
}

double totalBytesSentASecondAgo = 0;
void logSpeed() {
    int logIndex = 0;
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        double bandwidth = (totalBytesSent-totalBytesSentASecondAgo)/1024/1024*8;
        totalBytesSentASecondAgo = totalBytesSent;
        std::time_t t = std::time(0);
        cout << bandwidth << " Mbit/s" << endl;
        outfile << logIndex++ << "," << bandwidth << "," << t << endl;
    }
}


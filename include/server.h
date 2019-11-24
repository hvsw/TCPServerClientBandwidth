//
//  server.h
//  TCPServer
//
//  Created by Henrique Valcanaia on 2019-11-24.
//  Copyright © 2019 Henrique Valcanaia. All rights reserved.
//

#ifndef server_h
#define server_h

	#include <fstream>
	#include <iostream>
	#include <netinet/in.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <string>
	#include <sys/socket.h>
	#include <thread>
	#include <unistd.h>

	// Note:
	// 1. The maximum send buffer size is 1 048 576 bytes.
	// 2. The default value of the SO_SNDBUF option is 32 767.
	// 3. For a TCP socket, the maximum length that you can specify is 1 GB.
	// 4. For a UDP or RAW socket, the maximum length that you can specify is the smaller of the following values:
	// 4.1 65 535 bytes (for a UDP socket) or 32 767 bytes (for a RAW socket).
	// 4.2 The send buffer size defined by the SO_SNDBUF option.
	// https://www.ibm.com/support/knowledgecenter/en/SSB23S_1.1.0.12/gtpc2/cpp_send.html
	// #define PKT_SIZE 65568
	// Let's optimize transfer speed
	#define PKT_SIZE 1024*1024
	// #define PKT_SIZE 64*1024

	void readData();
	void logSpeed();

#endif /* server_h */
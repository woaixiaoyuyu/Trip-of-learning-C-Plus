//
//  client_test.cpp
//  myTTT
//
//  Created by xiaoyuyu on 2022/3/20.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // 大量针对系统调用的封装
#include <arpa/inet.h>
#include <sys/socket.h>


void error_handling(std::string message) {
    std::cout << message << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    ssize_t str_len;
    
    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        error_handling("socket error");
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect error");
    }
    
    str_len = read(sock, message, sizeof(message) - 1);
    if (str_len == -1) {
        error_handling("read error");
    }
    
    printf("Message from server : %s\n", message);
    close(sock);
    return 0;
}

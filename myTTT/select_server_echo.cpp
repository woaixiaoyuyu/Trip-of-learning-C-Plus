//
//  server_test.cpp
//  myTTT
//
//  Created by xiaoyuyu on 2022/3/22.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

// 利用select函数实现的回声并发服务端

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // 大量针对系统调用的封装
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100

void error_handling(std::string message) {
    std::cout << message << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    int serv_sock;
    int clnt_scok;
    int str_len, i;
    static int idx = 0;
    char message[BUF_SIZE];
    
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    
    struct timeval timeout;
    fd_set reads, cpy_reads;
    int fd_max, fd_num;
    
    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1) {
        error_handling("socket error");
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind error");
    }
    
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen error");
    }
    
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;
    
    clnt_addr_size = sizeof(clnt_addr);
    
    while (true) {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        
        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
            break;
        if (fd_num == 0)
            continue;
        
        for (i = 0; i < fd_max + 1; i++) {
            if (FD_ISSET(i, &cpy_reads)) {
                if (i == serv_sock) {
                    // connection request
                    clnt_scok = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
                    if (clnt_scok == -1) {
                        error_handling("accept error");
                    } else {
                        idx++;
                        printf("Connected client number is %d\n", idx);
                    }
                    FD_SET(clnt_scok, &reads);
                    if (fd_max < clnt_scok)
                        fd_max = clnt_scok;
                } else {
                    // read message
                    str_len = read(clnt_scok, message, BUF_SIZE);
                    if (str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d\n", i);
                    } else {
                        write(clnt_scok, message, str_len);
                    }
                }
            }
        }
    }
    
    close(serv_sock);
    
    return 0;
}

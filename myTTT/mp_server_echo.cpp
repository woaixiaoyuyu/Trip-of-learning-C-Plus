//
//  server_test.cpp
//  myTTT
//
//  Created by xiaoyuyu on 2022/3/21.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

// 把之前的回声服务器改成并发服务器

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // 大量针对系统调用的封装
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

#define BUF_SIZE 30

// 处理错误
void error_handling(std::string message) {
    std::cout << message << std::endl;
    exit(1);
}

// 子进程结束时的处理函数
void read_childproc(int sig) {
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("removed proc id: %d\n", pid);
}

int main(int argc, char* argv[]) {
    int serv_sock;
    int clnt_scok;
    int str_len;
    static int i = 0;
    char message[BUF_SIZE];
    
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    
    pid_t pid;
    struct sigaction act;
    int state;
    
    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);
    
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
    
    clnt_addr_size = sizeof(clnt_addr);
    
    while(true) {
        clnt_scok = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_scok == -1) {
            continue;
        } else {
            i++;
            printf("Connected client %d\n", i);
        }
        
        pid = fork();
        if (pid == -1) {    // 失败时返回-1
            close(clnt_scok);
            continue;
        }
        
        // 子进程运行区域
        if (pid == 0) {
            close(serv_sock);   // 把从父进程复制的serv socket关闭
            // 把收到的传回去
            while ((str_len = read(clnt_scok, message, BUF_SIZE)) != 0) {
                // std::cout << message << std::endl;
                // std::cout << str_len << std::endl;
                write(clnt_scok, message, str_len);
            }
            close(clnt_scok);
            std::cout << "client disconnected..." << std::endl;
            return 0;
        } else {
            close(clnt_scok);
        }
    }
    
    close(serv_sock);
    
    return 0;
}

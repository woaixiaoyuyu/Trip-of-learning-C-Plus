//
//  main.cpp
//  myTTTServer
//
//  Created by xiaoyuyu on 2022/3/22.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

/**
 井字游戏
 服务端
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // 大量针对系统调用的封装
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>
#include <vector>
#include <queue>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 300
#define width 3

static int serv_sock = 0;
static struct sockaddr_in serv_addr;
static std::deque<int> clnt_queue;
static std::deque<int> gaming_queue;
static int players_number = 0;
static int playing_number = 0;
static char first = 'x';
static char second = 'o';
pid_t pid;

// 处理错误
void error_handling(std::string message) {
    std::cout << message << std::endl;
    exit(1);
}

// one turn 一名玩家的一次交互
void one_turn(int fd_max, int clnt_sock, fd_set cpy_reads, char* str, int len, struct timeval timeout, bool& flag, char sym, char chess_board[][3]) {
    write(clnt_sock, str, len);
    // 等待clnt_a的选择
    int fd_num;
    char choice[5];
    ssize_t str_len;
    while (true) {
        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, NULL)) == -1) {
            printf("fd_num : %d.\n", fd_num);
            break;
        }
        if (fd_num == 0)
            continue;
        if (FD_ISSET(clnt_sock, &cpy_reads)) {
            str_len = read(clnt_sock, choice, 4);
            if (str_len == 0) {
                FD_CLR(clnt_sock, &cpy_reads);
                close(clnt_sock);
                printf("closed client: %d\n", clnt_sock);
            } else {
                std::cout << "finish one strp." << std::endl;
                int x = choice[0] - '0', y = choice[2] - '0';
                chess_board[x][y] = sym;
                flag = !flag;
                std::cout << flag << std::endl;
                break;
            }
        }
        sleep(2);
    }
}

int main(int argc, const char * argv[]) {
        
    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1) {
        error_handling("socket error");
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    //设置端口复用
    int opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEPORT, (void*)&opt, sizeof(opt));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind error");
    }
    
    if (listen(serv_sock, 2) == -1) {
        error_handling("listen error");
    }
    
    // 客户端相关信息
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    int clnt_sock;
    clnt_addr_size = sizeof(clnt_addr);
    
    while(true) {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            continue;
        } else {
            players_number++;
            printf("A new player has connected.\n");
            clnt_queue.emplace_back(clnt_sock); // 加入等待队列
            gaming_queue.emplace_back(clnt_sock);   // 父进程也准备一份客户端套接字，在父进程中销毁，防止僵尸进程
            // int clnt_a = clnt_queue.front();
            // char str[] = "hello";
            // write(clnt_a, str, sizeof(str));
        }
        
        if (players_number >=2) {
            // 人数大于2即可开始游戏
            players_number -= 2;
            playing_number = 2;
            
            close(serv_sock);   // 把从父进程复制的serv socket关闭
             
            // 对每个子进程采取I/O复用select
            struct timeval timeout;
            fd_set reads, a_reads, b_reads;
            int fd_max = serv_sock;
            FD_ZERO(&reads);
            FD_ZERO(&a_reads);
            FD_ZERO(&b_reads);
            FD_SET(serv_sock, &reads);
            timeout.tv_sec = 5;
            timeout.tv_usec = 5000;
             
            // 获取对战双方的客户端套接字
            int clnt_a = clnt_queue.front();
            if (clnt_a > fd_max)
                fd_max = clnt_a;
            FD_SET(clnt_a, &a_reads);
            FD_SET(clnt_a, &reads);
            clnt_queue.pop_front();
            
            int clnt_b = clnt_queue.front();
            if (clnt_b > fd_max)
                fd_max = clnt_b;
            FD_SET(clnt_b, &b_reads);
            FD_SET(clnt_b, &reads);
            clnt_queue.pop_front();
             
            // 激活服务端的棋盘，客户端会自己激活本地的棋盘，服务端的棋盘没必要展示
            char chess_board[width][width];
            memset(chess_board, '?', sizeof(chess_board));
             
            // 与客户端开始交互
            bool flag = true;   // true 和clnt_a互动，反之则是clnt_b
            char str2[] = "now it's your turn.";
            while (true) {
                if (flag) {
                    one_turn(fd_max, clnt_a, a_reads, str2, 30,timeout, flag, first, chess_board);
                    // 刷新客户端的棋盘
                    write(clnt_a, chess_board, 9);
                } else {
                    one_turn(fd_max, clnt_b, b_reads, str2, 30,timeout, flag, second, chess_board);
                    write(clnt_a, chess_board, 9);
                }
                // 判断输赢
                
                sleep(2);
            }
            
        close(clnt_a);
        close(clnt_b);
        std::cout << "client disconnected..." << std::endl;
        return 0;
        }
        
        
        
    }
    
    close(serv_sock);
    
    return 0;
}

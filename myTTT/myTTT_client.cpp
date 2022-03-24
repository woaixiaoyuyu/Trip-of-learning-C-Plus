//
//  client_test.cpp
//  myTTT
//
//  Created by xiaoyuyu on 2022/3/22.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // 大量针对系统调用的封装
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define width 3

void error_handling(std::string message) {
    std::cout << message << std::endl;
    exit(1);
}

// 展示棋盘
void show_chess_board(char chess_board[][3]) {
    for (int i = 0; i < width; i++) {
        std::cout << "=====================" <<std::endl;
        for (int j = 0; j < width; j++) {
            std::cout << chess_board[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << "=====================" <<std::endl;
}

int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    char chess_board[width][width];
    memset(chess_board, '?', sizeof(chess_board));
    
    ssize_t str_len, recv_len, recv_cnt;
    
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
    } else {
        printf("Connected......\n");
    }
    
    // 接受欢迎
    std::cout << "welcome to xiaoyuyu's Tic-Tac-Toe." << std::endl;
    
    // 展示棋盘
    show_chess_board(chess_board);
    
    // 接收消息，进行对战
    char choice[5];
    int x = -1, y = -1;
    while(true) {
        recv_len = read(sock, message, BUF_SIZE - 1);
        if (recv_len < 1)
            break;
        
        if (strcmp(message, "now it's your turn.") == 0) {
            while (true) {
                printf("input the x and y of your choice(etc. 0 2) : ");
                scanf("%d %d", &x,&y);
                if (x < 0 || x > 2 || y < 0 || y > 2 || chess_board[x][y] != '?') {
                    printf("wrong choice, try again.\n");
                    sleep(1);
                    continue;
                }
                choice[0] = x + '0';
                choice[2] = y + '0';
                write(sock, choice, 4);
                break;
            }
        } else if (strcmp(message, "you win") == 0) {
            printf("niubility.\n");
            printf("you wiil gain overnight famous.\n");
            break;
        } else if (strcmp(message, "you lose") == 0) {
            printf("vegetable.\n");
            break;
        } else if (recv_len == 9) {
            memcpy(chess_board, message ,9);
            show_chess_board(chess_board);
            printf("wait another player...\n");
        }
        sleep(2);
    }
    
    close(sock);
    return 0;
}

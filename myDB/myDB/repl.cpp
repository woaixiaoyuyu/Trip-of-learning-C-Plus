//
//  main.cpp
//  myDB
//
//  Created by xiaoyuyu on 2022/2/22.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

class myBuffer {
public:
    string buff;
    size_t bufferSize;
    ssize_t inputSize;
    
    myBuffer() : bufferSize(0),inputSize(0) {
        buff = "";
    }
    
    ~myBuffer() {}
};

myBuffer* newInputBuffer() {
    myBuffer * buffer = new myBuffer;
    return buffer;
}

void printPrompt() {
    cout << "DB > ";
}

void readInput(myBuffer * buff) {
    cin >> buff->buff;
    int len = (int)buff->buff.size();
    buff->bufferSize = buff->inputSize = len;
}

int main(int argc, const char * argv[]) {
    // 创建缓冲区
    myBuffer * buff = newInputBuffer();
    while (true) {
        printPrompt();
        // 读取信息
        readInput(buff);
        // 处理信息
        string command = buff->buff;
        if (command == ".exit") {
            // 退出
            exit(EXIT_SUCCESS);
        } else {
            cout << "Unrecognized command : " << buff->buff << endl;
        }
    }
    return 0;
}

////
////  Compiler and Virtual Machine.cpp
////  myDB
////
////  Created by xiaoyuyu on 2022/2/23.
////  Copyright © 2022 xiaoyuyu. All rights reserved.
////
//
//#include <stdio.h>
//#include <string.h>
//#include <iostream>
//
//using namespace std;
//
//typedef enum {
//  META_COMMAND_SUCCESS,
//  META_COMMAND_UNRECOGNIZED_COMMAND
//} MetaCommandResult;
//
//typedef enum {
//    PREPARE_SUCCESS,
//    PREPARE_UNRECOGNIZED_STATEMENT
//} PrepareResult;
//
//typedef enum {
//    STATEMENT_INSERT,
//    STATEMENT_SELECT
//} StatementType;
//
//typedef struct {
//  StatementType type;
//} Statement;
//
//class myBuffer {
//public:
//    string buff;
//    size_t bufferSize;
//    ssize_t inputSize;
//
//    myBuffer() : bufferSize(0),inputSize(0) {
//        buff = "";
//    }
//
//    ~myBuffer() {}
//};
//
//myBuffer* newInputBuffer() {
//    myBuffer * buffer = new myBuffer;
//    return buffer;
//}
//
//void printPrompt() {
//    cout << "DB > ";
//}
//
//void readInput(myBuffer * buff) {
//    getline(cin,buff->buff);
//    int len = (int)buff->buff.size();
//    buff->bufferSize = buff->inputSize = len;
//}
//
//MetaCommandResult do_meta_command(string command) {
//    cout << command[2] << endl;
//    if (command == ".exit") {
//        // 退出
//        exit(EXIT_SUCCESS);
//    } else
//        return META_COMMAND_UNRECOGNIZED_COMMAND;
//}
//
//PrepareResult prepare_statement(string command, Statement& statement) {
//    if (command.substr(0,6) == "select") {
//        statement.type = STATEMENT_SELECT;
//        return PREPARE_SUCCESS;
//    }
//    if (command.substr(0,6) == "insert") {
//        statement.type = STATEMENT_INSERT;
//        return PREPARE_SUCCESS;
//    }
//    return PREPARE_UNRECOGNIZED_STATEMENT;
//}
//
//void execute_statement(Statement& statement) {
//    switch (statement.type) {
//        case STATEMENT_SELECT:
//            cout << "This is where we would do a select." << endl;
//            break;
//        case STATEMENT_INSERT:
//            cout << "This is where we would do an insert." << endl;
//            break;
//    }
//}
//
//int main(int argc, const char * argv[]) {
//    // 创建缓冲区
//    myBuffer * buff = newInputBuffer();
//    while (true) {
//        printPrompt();
//        // 读取信息
//        readInput(buff);
//        // 处理信息
//        string command = buff->buff;
//        // Non-SQL statements like .exit are called “meta-commands”. They all start with a dot, so we check for them and handle them in a separate function.
//        // 判断指令是否存在，或者说指令是否符合标准
//        if (command[0] == '.') {
//            switch (do_meta_command(command)) {
//                case META_COMMAND_SUCCESS :
//                    continue;
//                case META_COMMAND_UNRECOGNIZED_COMMAND :
//                    cout << "Unrecognized command : " << command << endl;
//                    continue;
//            }
//        }
//        // Next, we add a step that converts the line of input into our internal representation of a statement. This is our hacky version of the sqlite front-end.
//        // 将指令转换成字节码
//        Statement statement;
//        switch (prepare_statement(command,statement)) {
//            case PREPARE_SUCCESS:
//                break;
//            case PREPARE_UNRECOGNIZED_STATEMENT:
//                cout << "Unrecognized keyword at start of " << command << endl;
//                continue;
//        }
//
//        // Lastly, we pass the prepared statement to execute_statement. This function will eventually become our virtual machine.
//        // 虚拟机执行指令对应的字节码
//        execute_statement(statement);
//        cout << "Executed." << endl;
//    }
//    return 0;
//}

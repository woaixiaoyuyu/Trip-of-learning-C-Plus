//
//  An In-Memory, Append-Only, Single-Table Database.cpp
//  myDB
//
//  Created by xiaoyuyu on 2022/2/24.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;

const int COLUMN_USERNAME_SIZE = 32;
const int COLUMN_EMAIL_SIZE = 255;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
} StatementType;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} ExecuteResult;

const int MAX_PAGES = 100;
const int PAGE_SIZE = 4096;

// struct of table in memory
class Table {
public:
    size_t num_rows;    // the number of total rows in the table
    void* page[MAX_PAGES];  // an array of pages, each pointer reflects a page
    Table() : num_rows(0) {
        for (int i = 0; i < MAX_PAGES; i++)
            page[i] = nullptr;
    }
    
    ~Table() {
        for (int i = 0; i < MAX_PAGES; i++)
            if (page[i] != nullptr)
                delete page[i];
    }
};

// struct of a row
// reflect data of statement, etc.id , username, email
struct Row {
    string id;
    string username;
    string email;
};

#define size_of_attribute(Struct,Attribute) sizeof(((Struct*)0)->Attribute)
const int ID_SIZE = size_of_attribute(Row, id);
const int USERNAME_SIZE = size_of_attribute(Row, username);
const int EMAIL_SIZE = size_of_attribute(Row, email);
const int ID_OFFSET = 0;
const int USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const int EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const int ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const int ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const int TABLE_MAX_ROWS = ROWS_PER_PAGE * MAX_PAGES;


class Statement {
public:
    Row* row;   // a pointer to a row
    StatementType type; //type of a statement, etc.insert, select
    Statement() : type(STATEMENT_SELECT) {
        row = new Row;
    }
    ~Statement() {
        delete row;
    }
};

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

void closeBuffer(myBuffer * buffer) {
    delete buffer;
    buffer = nullptr;
}

void printPrompt() {
    cout << "DB > ";
}

void readInput(myBuffer * buff) {
    getline(cin,buff->buff);
    int len = (int)buff->buff.size();
    buff->bufferSize = buff->inputSize = len;
}

MetaCommandResult do_meta_command(string command) {
    return META_COMMAND_UNRECOGNIZED_COMMAND;   // 目前除了.exit，识别不了任何指令
}

PrepareResult prepare_statement(string command, Statement& statement) {
    // 将对应的数据插入到Statement的结构中
    istringstream my_stream(command);
    string type;
    my_stream >> type;
    int idx = 0;
    string temp[3];   // 临时存放id,username,email三个元素
    if (type == "select") {
        statement.type = STATEMENT_SELECT;
    }
    if (type == "insert") {
        statement.type = STATEMENT_INSERT;
    }
    while (my_stream) {
        my_stream >> temp[idx];
        // cout << temp[idx] << endl;
        idx++;
    }
    // cout << idx << endl;
    if (idx < 3) {
        return PREPARE_SYNTAX_ERROR;
    } else {
        statement.row->id = temp[0];
        statement.row->username = temp[1];
        statement.row->email = temp[2];
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

// 求行所在地址
void* row_slot(Table table) {
    // 确定应该插入在第几页
    int page_num = table.num_rows / ROWS_PER_PAGE;
    // 判断是否需要创建新的页
    void* page = table.page[page_num];
    if (page == nullptr) {
        table.page[page_num] = operator new(PAGE_SIZE);
        page = table.page[page_num];
    }
    // 判断应该插入的页内偏移量
    int row_offset = table.num_rows % ROWS_PER_PAGE;
    int byte_offset = row_offset * ROW_SIZE;
    return (char*)page + byte_offset;
}

// 序列化 insert
void serialize_row(Row* source, void* destination) {
    memcpy((char*)destination + ID_OFFSET, &(source->id), ID_SIZE);
    memcpy((char*)destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy((char*)destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

// 反序列化 select

ExecuteResult execute_insert(Statement& statement, Table& table) {
    // 判断是否还有存储的空间
    if (table.num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }
    // 需要获得当前数据应该插入的位置
    void* destination = row_slot(table);
    // 进行序列话，也就是插入数据
    serialize_row(statement.row, destination);
    table.num_rows++;
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement& statement, Table table) {
    return EXECUTE_TABLE_FULL;
}

ExecuteResult execute_statement(Statement& statement, Table table) {
    switch (statement.type) {
        case STATEMENT_SELECT:
            return execute_select(statement, table);
            break;
        case STATEMENT_INSERT:
            return execute_insert(statement,table);
            break;
    }
}

int main(int argc, const char * argv[]) {
    // 创建用来存放内存中数据的table
    Table table;
    // 创建缓冲区
    myBuffer * buff = newInputBuffer();
    while (true) {
        printPrompt();
        // 读取信息
        readInput(buff);
        // 处理信息
        string command = buff->buff;
        // Non-SQL statements like .exit are called “meta-commands”. They all start with a dot, so we check for them and handle them in a separate function.
        // 判断指令是否存在，或者说指令是否符合标准
        if (command[0] == '.') {
            if (command == ".exit") {
                // 退出
                closeBuffer(buff);
                exit(EXIT_SUCCESS);
            }
            switch (do_meta_command(command)) {
                case META_COMMAND_SUCCESS :
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND :
                    cout << "Unrecognized command : " << command << endl;
                    continue;
            }
        }
        // Next, we add a step that converts the line of input into our internal representation of a statement. This is our hacky version of the sqlite front-end.
        // 将指令转换成字节码
        Statement statement;
        switch (prepare_statement(command,statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                cout << "Unrecognized keyword at start of " << command << endl;
                continue;
            case PREPARE_SYNTAX_ERROR:
                cout << "Syntax error. Could not parse statement." << endl;
                continue;
        }

        // Lastly, we pass the prepared statement to execute_statement. This function will eventually become our virtual machine.
        // 虚拟机执行指令对应的字节码
        switch(execute_statement(statement,table)) {
            case (EXECUTE_SUCCESS):
                cout << "Executed." << endl;
                break;
            case (EXECUTE_TABLE_FULL):
                cout << "Error: Table full." << endl;
                break;
        }
    }
    return 0;
}

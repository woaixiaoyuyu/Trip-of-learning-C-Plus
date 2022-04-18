//
//  CommandPattern.cpp
//  myDesignPatterns
//
//  Created by xiaoyuyu on 2022/4/18.
//  Copyright © 2022 xiaoyuyu. All rights reserved.
//

// 这一部分代码直接从GOF书中获得，要是需要更详细的解释，可以直接去看书，我稍微改了一点让他可以运行。

#include <stdio.h>
#include <iostream>

class Command {
public:
    virtual ~Command() {};
    virtual void Execute() = 0;
protected:
    Command() {};
};

/**
 OpenCommand opens a document whose name is supplied by the user. An OpenCommand must be passed an
 Application object in its constructor. AskUser is an implementation routine that prompts the user for the
 name of the document to open.
 */

//class OpenCommand : public Command {
//public:
//    OpenCommand(Application*);
//    virtual void Execute();
//protected:
//    virtual const char* AskUser();
//private:
//    Application* _application;
//    char* _response;
//};
//
//OpenCommand::OpenCommand (Application* a) {
//    _application = a;
//}
//
//void OpenCommand::Execute () {
//    const char* name = AskUser();
//    if (name != 0) {
//        Document* document = new Document(name);
//        _application->Add(document);
//        document->Open();
//    }
//}

template <class Receiver>
class SimpleCommand : public Command {
public:
    // using Action = void (*)(); error
    using Action = void(Receiver::*)();
    // typedef void (Receiver::* Action)(); the same
    SimpleCommand(Receiver* r, Action a) : _receiver(r), _action(a) {}
    virtual void Execute();
private:
    Action _action;
    Receiver* _receiver;
};

template <class Receiver>
void SimpleCommand<Receiver>::Execute() {
    (_receiver->*_action)();
}

class MyClass {
public:
    void Action() {
        std::cout << "MyClass action." <<std::endl;
    };
};

void dummy() {
    MyClass* receiver = new MyClass;
    Command* aCommand = new SimpleCommand<MyClass>(receiver, &MyClass::Action);
    aCommand->Execute();
}

//int main() {
//    dummy();
//}

#include <iostream>
#include <string.h>

using namespace std;

class Person {
public:
	static int wealth;
	int sex;
	int age;
	char* name;	

public:
	virtual void print() {
		cout << "name is : " << name << endl;
	}

public:
	Person() {}

	Person(int sex, int age, char* name) : sex(sex), age(age) {
		this->name = new char[30];
		strcpy(this->name, name);
	}

	virtual ~Person() {
		delete name;
		name = nullptr;
	}
};

class Student : public Person {
public:
	int number;

public:
	void print() {
		cout << "student number is : " << number << endl;
	}
};


int main(int argc, char const *argv[]) {
	
	cout << sizeof(Person) << endl;	// 8 + 4 + 4 + 8 = 24
	cout << sizeof(Student) << endl;	// 24 + 4 + 4padding = 32

	Person p(1, 18, "xiaoyuyu");

	Student q;

	cout << "p address : " << &p << endl;
	cout << "q address : " << &q << endl;

	return 0;
}
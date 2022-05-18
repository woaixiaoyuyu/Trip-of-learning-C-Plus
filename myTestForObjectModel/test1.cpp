#include <iostream>
#include <string.h>

using namespace std;

class Person {
public:
	int sex;
	int age;
	char* name;	

public:
	void print() {
		cout << "name is : " << name << endl;
	}
};

class Student : public Person {
public:
	int number;
};


int main(int argc, char const *argv[]) {
	
	cout << sizeof(Person) << endl;	// 4 + 4 + 8 = 16
	cout << sizeof(Student) << endl;	// 16 + 4 + 4padding = 24
	return 0;
}
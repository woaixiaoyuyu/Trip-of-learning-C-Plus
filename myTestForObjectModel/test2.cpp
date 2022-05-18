#include <iostream>
#include <string.h>

using namespace std;

class X {};
class Y : public X {};
class Z : public X {};
class A : public Y, public Z {};

class x {};
class y : virtual public x {};
class z : virtual public x {};
class a : virtual public y, public z {};


int main(int argc, char const *argv[]) {
	
	cout << sizeof(X) << endl;	// 1
	cout << sizeof(Y) << endl;	// 1
	cout << sizeof(Z) << endl;	// 1
	cout << sizeof(A) << endl;	// 2

	cout << sizeof(x) << endl;	// 1
	cout << sizeof(y) << endl;	// 8
	cout << sizeof(z) << endl;	// 8
	cout << sizeof(a) << endl;	// 16
	return 0;
}
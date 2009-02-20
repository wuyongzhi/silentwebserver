#include <iostream>

using namespace std;

class A {
public:
	A() {
		cout << "A created" << endl;
	}

	~A() {
		cout << "A destroyed" << endl;
	}
};

class B {
public:
	A& a;

	B(A& _a) : a(_a) {
	}
	
	~B() {
	}
};





int main() {
	A a;
	B b(a);

	return 0;
}

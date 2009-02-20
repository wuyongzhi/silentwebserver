
#include <iostream>

using namespace std;

//using std:out;
//using std:endl;

class A {
	public:
	A() {
		cout << "A created" << endl;
	}

	~A() {
		cout << "A free" << endl;
	}
};
typedef A arr[10];

int main() {
	A* pa = new arr;
	delete[] pa; // 这儿是使用 delete[] 而不是 delete 来删除 pa
}


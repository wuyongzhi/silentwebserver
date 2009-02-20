#include <iostream>

using namespace std;

struct A{
	int i;
	int j;
	float f;
};

int main() {
	cout << sizeof(A&) << endl;
	
	return 0;
}

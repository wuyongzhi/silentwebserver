#include <iostream>
	
using namespace std;
	


class A {
public:
	template<typename T>
	T call(T value) {
		return ++value;
	}
};


int main() {
	A a;

	a.call(21);
	a.call(32.0f);

	return 0;	
}

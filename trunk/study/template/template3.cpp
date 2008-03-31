#include <iostream>

using namespace std;
 
template<typename T>
class A {
public:

	template<typename T2>
	void p(T2 value);

	void p(char value);
};

template<typename T>	//类模板参数 	
template<typename T2> //类成员模板参数
void A<T>::p(T2 value) {
	cout << "通用" << endl;
}

template<typename T>
void A<T>::p(char value) {
	cout << "char value: " << value << endl;
}

int main() {

	A<int> a;
	a.p(32);
	a.p((char)32);

	
	return 0;
}

#include <iostream>

using namespace std;

enum EnumType {
	EnumValue1 = 1,
	EnumValue2 = 2
};

template <typename T, EnumType enumvalue>
class A {
public :
	typedef T MyType;

	void p();
	void p2(T value) {
		cout << value << endl;
	}

};

template<typename T, EnumType enumvalue>
void A<T, enumvalue>::p() {
	cout << "通用模板" << endl;
}

template<>
void A<int, EnumValue1>::p() {
	cout << "int instance" << endl;
}


template<>
void A<char, EnumValue2>::p() {
	cout << "char instance" << endl;
}

int main () {
	A<int, EnumValue1> int_a;
	A<int, EnumValue2> int_a2;

	int_a.p();
	int_a2.p();

}

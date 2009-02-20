#include <iostream>

using namespace std;

int main() {
#ifdef __LINUX__	
	cout << "__LINUX__" << endl;
#endif
	
#ifdef __UNIX__	
	cout << "__UNIX__" << endl;
#endif

#ifdef __linux__	
	cout << "__linux__" << endl;
#endif
	
#ifdef __unix__	
	cout << "__unix__" << endl;
#endif


	return 0;
}

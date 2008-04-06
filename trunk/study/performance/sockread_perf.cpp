#include <iostream>
#include <ctime>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>





int main() {
	sleep(1);
	using namespace std;
	const unsigned short PORT = 8089;
	const unsigned int BUF_SIZE = 1024 * 190;

	int s = socket(AF_INET,SOCK_STREAM, 0);

	sockaddr_in sock_addr = {0};
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(PORT);
	sock_addr.sin_addr.s_addr = htonl(0);

	if (::bind(s,reinterpret_cast<sockaddr*>(&sock_addr),sizeof(sock_addr)) < 0) {
		cout << "bind error" << endl;
		close(s);
		exit(0);
	}

	if (::listen(s, 10) < 0) {
		cout << "listen error" << endl;
		close(s);
		exit(0);
	}

	cout << "server: listening at " << PORT << endl;

	int c1 = socket(AF_INET,SOCK_STREAM,0);
	sockaddr_in remote;

	remote.sin_family = AF_INET;
	remote.sin_port = htons(PORT);
	remote.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if (0 != ::connect(c1, reinterpret_cast<sockaddr*>(&remote), sizeof(remote))) {
		cout << "client: connect error! " << endl;
		exit(0);
	}
	cout << "client: connected" << endl;
	
	sockaddr_in local;
	socklen_t len = sizeof(local);
	int c2 = ::accept(s, reinterpret_cast<sockaddr*>(&local), &len);
	if (c2 < 0) {
		cout << "server: accept error! " << endl;
		exit(0);
	}
	cout << "server: accepted. socket fd=" << c2 << endl;


	int size, result;
	socklen_t socklen=sizeof(size);
	size = 1024 * 1;
	//setsockopt(c2, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	result = getsockopt(c2, SOL_SOCKET, SO_RCVBUF, &size, &socklen);
	cout << "server: recv buffer size=" << size << endl;
	
	
	char buf[BUF_SIZE];
	result = write(c1, buf, BUF_SIZE);
	if (result <= 0) {
		cout << "client: write error!" << endl;
		exit(0);
	}
	cout << "client: write " << result << " bytes" << endl;

	socklen=sizeof(size);
	result = getsockopt(c2, SOL_SOCKET, SO_RCVBUF, &size, &socklen);
	cout << "server: recv buffer size=" << size << endl;


	char buf2[1];
	clock_t start = clock();
	for (int i=0; i<(BUF_SIZE); ++i) {
		read(c2, buf2, 1);
	}
	clock_t end = clock();
	cout << "used cpu clock: " << end - start << endl;
	cout << "clocks per sec: " << CLOCKS_PER_SEC << endl;
	cout << "used time: " << (((double)(end-start))/CLOCKS_PER_SEC) << endl;

	socklen=sizeof(size);
	result = getsockopt(c2, SOL_SOCKET, SO_RCVBUF, &size, &socklen);
	cout << "server: recv buffer size=" << size << endl;

	
	close(s);
	close(c1);
	close(c2);

	return 0;
}

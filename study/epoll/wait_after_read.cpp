#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>


const unsigned short PORT = 8089;
#define EPOLLRDHUP 0x2000
using namespace std;


int main() {
	int error;
	int listener = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listener < 0) {
		puts ("socket create failed");
		return 0;
	}
	
	sockaddr_in sock_addr = {0};
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = ::htons(PORT);
	sock_addr.sin_addr.s_addr = ::htonl(0);

	if ( error=::bind(listener, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr)) < 0)  {
		printf ("Can NOT bind port %d \n", PORT);
		return 0;
	}

	if ( error=::listen(listener, 10) < 0) {
		printf ("Can NOT listen socket. backlog=%d \n", 10);
		return 0;
	}
	
	// 创建epoll


	int epoll = epoll_create( 10);
	if (epoll <=0 ) {
		cout << "epoll_create error!" << endl;
		return 0;
	}
	

	sockaddr_in client_address = {0};
	socklen_t size = sizeof(client_address);

	int connection = accept(listener, reinterpret_cast<sockaddr*>(&client_address), &size);

	epoll_event event = {0};
	event.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
	if (connection > 0) {

		error = epoll_ctl(epoll, EPOLL_CTL_ADD, connection, &event);
		if (error != 0) {
			cout << "epoll_ctl error" << endl;
			return 0;
		}
		
		// 休息 2 秒
		sleep(2);

		// 读取数据
		int recv_len;
		char buf[512];
		recv_len = recv(connection, buf, sizeof(buf), 0);
		if (recv_len < 0) {
			cout << "recv can NOT  get data" << endl;
			return 1;
		}

		cout << "recv " << recv_len << " bytes." << endl;
		
		epoll_event events[1];

		int epoll_wait_result = epoll_wait (epoll, events, sizeof(events) / sizeof(epoll_event), 2000);
		cout << "epoll_wait return " << epoll_wait_result << endl;

		close(connection);
	}

	close(epoll);
	close(listener);

	return 0;
}





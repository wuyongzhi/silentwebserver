#ifndef _SILENT_ACCEPTOR_LINUX_H_
#define _SILENT_ACCEPTOR_LINUX_H_



// EPOLL_ALL_EVENTS 排除非事件的标志位, 如, EPOLLET(设为Edge Level Triggered) 和 EPOLLONESHOT(快照)
#ifndef EPOLL_ALL_EVENTS
#define EPOLL_ALL_EVENTS  (EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLPRI | EPOLLERR | EPOLLHUP)
#endif

namespace silent {



template<class C, class H>
bool Acceptor<C,H>::createReactor() {
	reactor = epoll_create(concurrentConnections + 1);
	return isValidHandle(reactor);
}

template<class C, class H>
bool Acceptor<C,H>::addListenerToReactor() {

	assert (isValidHandle(reactor));
	assert (isValidSocket(listener));

	epoll_event e = {0};
	e.events = EPOLL_ALL_EVENTS | EPOLLET;
	e.data.fd = listener;

	int err = epoll_ctl (this->reactor, EPOLL_CTL_ADD,
							listener, &e);
	return err==0;
}


std::ostream& operator<< (ostream& os, const epoll_event& e) {
	os << "epoll_event(";
	if (e.events & EPOLLIN) { os << "EPOLLIN "; }
	if (e.events & EPOLLOUT) { os << "EPOLLOUT "; }
	if (e.events & EPOLLRDHUP) { os << "EPOLLRDHUP "; }
	if (e.events & EPOLLPRI) { os << "EPOLLPRI "; }
	if (e.events & EPOLLERR) { os << "EPOLLERR "; }
	if (e.events & EPOLLHUP) { os << "EPOLLHUP "; }
	if (e.events & EPOLLET) { os << "EPOLLET "; }
	if (e.events & EPOLLONESHOT) { os << "EPOLLONESHOT "; }
	os << ")";
	return os;
}

template<class C, class H>
int Acceptor<C,H>::wait(Connections& opened,
						Connections& received,
						Connections& sent,
						Connections& closed,
						int timeout) {

	lastActivelyConnections = 
		epoll_wait(reactor, &epollEvents[0], epollEvents.size(), timeout);
	//int numbers = lastActivelyConnections;
	if (lastActivelyConnections > 0) {
		EpollEvents::iterator first=epollEvents.begin();
		EpollEvents::iterator end =first + lastActivelyConnections;
		
		for (; first!=end; ++first) {
			EpollEvents::reference e = *first;
			
			if (e.data.fd==listener)
				std::cout << "listener: ";
			else {
				ConnectionType *ct = reinterpret_cast<ConnectionType*>(e.data.ptr);
				cout << "fd(" << ct->sock_fd << "): ";
				
			}
			std::cout << e << endl;

			if (e.data.fd != listener) {
				//
				/**
					现在已经开始面临的问题：
					1、多线程如何同步控制，最大化性能。（有点难度）需要考虑，可伸缩性，可控制性，不要太复杂，性能最大地发挥，可调试。
					2、错误处理 （简单）					
					3、对象、内存的创建分配，销毁释放。（加速对象、内存的创建分配）
				*/
				ConnectionType* c = reinterpret_cast<ConnectionType*>(e.data.ptr);
				if (e.events | EPOLLIN) {
					// 读取数据.
				} else if (e.events | EPOLLOUT) {
					// 写数据

				} else { // err 什么的
					//
				}
				

			} else {
				socket_t sock_fd;
				sockaddr_in addr;
				socklen_t addr_len = sizeof(addr);

				do {
					sock_fd = accept(
						listener, reinterpret_cast<sockaddr*>(&addr), &addr_len);

					if (isValidSocket(sock_fd)) {
						
						ConnectionType* c = new ConnectionType;
							
						if (initSocket(sock_fd, c)) {
							opened.push_back(c);
						} else {
							delete (c);
							closeSocket(sock_fd);
						}

					} else {

						// 异步 accept 错误处理
						if (errno == EAGAIN) {
							break;
						} else {
							int err = errno;
							cout << "accept fatal error. errno=" << strerror(err) << endl;
							exit(err);
						}
					}
				}while (true);
			}
		}
	}
	
	return lastActivelyConnections;
}

//using template<class C,class H>Acceptor<C,H>::ConnectionPointer;

template<class C, class H> 
bool Acceptor<C,H>::initSocket(socket_t sock_fd, ConnectionType* c) {

	set_nonblocking(sock_fd);

	c->sock_fd = sock_fd;

	epoll_event e;
	e.events = EPOLL_ALL_EVENTS | EPOLLET;
	e.data.ptr = c;
	
	return (epoll_ctl (reactor, EPOLL_CTL_ADD, sock_fd, &e) == 0);
}


template<class C, class H> 
void Acceptor<C,H>::processClient(int events, ConnectionType *c) {
	

	
	
}





} // namespace silent


#endif

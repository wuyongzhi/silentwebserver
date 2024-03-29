#ifndef _SILENT_ACCEPTOR_LINUX_H_
#define _SILENT_ACCEPTOR_LINUX_H_



// EPOLL_ALL_EVENTS 排除非事件的标志位, 如, EPOLLET(设为Edge Level Triggered) 和 EPOLLONESHOT(快照)
#ifndef EPOLL_ALL_EVENTS
#define EPOLL_ALL_EVENTS  (EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLPRI | EPOLLERR | EPOLLHUP)
#endif

namespace silent {

/**
	模板参数说明：

	C - Connection 类
	
*/
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
	HttpConnectionAllocator connection_allocator;

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
				if (e.events & EPOLLIN) {
					/*
 44 struct iovec
 45   {
 46     void *iov_base;  Pointer to data.  
 47     size_t iov_len;  Length of data.  
 48   };
					*
					*/
					// 读取数据.
					char buf[1024];
					iovec v;
					v.iov_base = buf;
					v.iov_len = 1023;
					int readlen = readv(c->sock_fd, &v, 1);
					if (readlen > 0) {
						cout << "recved " << readlen << "bytes" << endl;
						buf[readlen] = '\0';
						cout << buf << endl;
					} else {
						int e = errno;
						cout << "readv error" << endl;
						cout << e << endl;
						cout << strerror(e) << endl;
					}
				} else if (e.events & EPOLLOUT) {
					// 写数据

				} else if (e.events & (EPOLLERR|EPOLLHUP)) { // err 什么的
					
				}
				

			} else {
				socket_t sock_fd;
				sockaddr_in addr;
				socklen_t addr_len = sizeof(addr);

				do {
					sock_fd = accept(
						listener, reinterpret_cast<sockaddr*>(&addr), &addr_len);

					if (isValidSocket(sock_fd)) {
						
						ConnectionType* c = new(connection_allocator) ConnectionType;
						
						if (initSocket(sock_fd, c) == 0) {
							opened.push_back(c);
						} else {
							c->~ConnectionType();
							connection_allocator.deallocate(c);
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
int Acceptor<C,H>::initSocket(socket_t sock_fd, ConnectionType* c) {

	set_nonblocking(sock_fd);

	c->sock_fd = sock_fd;

	epoll_event e;
	e.events = EPOLL_ALL_EVENTS | EPOLLET;
	e.data.ptr = c;
	
	return epoll_ctl (reactor, EPOLL_CTL_ADD, sock_fd, &e);
}


template<class C, class H> 
void Acceptor<C,H>::processClient(ConnectionType *c) {
	

	
	
}





} // namespace silent


#endif

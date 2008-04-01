#ifndef _SILENT_ACCEPTOR_LINUX_H_
#define _SILENT_ACCEPTOR_LINUX_H_

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

	epoll_event event = {0};
	event.events = EPOLLIN;
	event.data.fd = listener;

	int err = epoll_ctl (this->reactor, EPOLL_CTL_ADD,
							listener, &event);
	return err==0;
}

// both are host byte order.
template<class C, class H>
bool Acceptor<C,H>::start(uint32_t address, uint16_t port) {

	//已经绑定,并且reactor已创建,则认为已经启动
	if (isRunning())
		return true;

	if (bind_internal(address, port, backlog) &&
		createReactor() &&
		addListenerToReactor()) {
		return true;
	}

	closeHandles();
	return false;
}

template<class C, class H>
int Acceptor<C,H>::wait(Connections& opened,
						Connections& received,
						Connections& sent,
						Connections& closed,
						int timeout) {

	//int result = epoll_wait(reactor,
	return 0;
}


template<class C, class H>
void Acceptor<C,H>::growEpollEventSize {
	if (epollEvents.capacity() < concurrentConnections) {
		EpollEvents::size_type old_capacity = epollEvents.capacity();
		EpollEvents::size_type new_capacity = old_capacity != 0 ? 2 * old_capacity : 1;
		if (new_capacity <= old_capacity)
			new_capacity = epollEvents.max_size();
		if (new_capacity > concurrentConnections)
			new_capacity = concurrentConnections;
	}
}



} // namespace silent


#endif

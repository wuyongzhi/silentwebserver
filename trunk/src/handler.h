#ifndef _XF_HANDLER_H_
#define _XF_HANDLER_H_	1


namespace xf {


template<class _ConnectionType>
class Handler {
public:
	typedef _ConnectionType ConnectionType;

	void created(ConnectionType cn) {}
	void received(ConnectionType cn) {}
	void sent(ConnectionType cn) {}
	void closed(ConnectionType cn) {}
};
}
#endif

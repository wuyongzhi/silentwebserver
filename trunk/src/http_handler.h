#ifndef _XF_HTTP_HANDLER_H_
#define _XF_HTTP_HANDLER_H_		1

#include "http_connection.h"
#include "handler.h"

namespace xf {

template<>
class Handler<HttpConnection> {
public:
	typedef HttpConnection ConnectionType; 

	void created(ConnectionType conn) {}
	void received(ConnectionType conn) {}
	void sent(ConnectionType conn) {}
	void closed(ConnectionType conn) {}

};


}
#endif

#ifndef _XF_HTTP_CONNECTION_H_
#define _XF_HTTP_CONNECTION_H_	1

#include "connection.h"

namespace xf {


class HttpConnection : public Connection {
public:
	HttpConnection() {}
	~HttpConnection() {}
};
}

#endif

#ifndef _SILENT_HTTP_CONNECTION_H_
#define _SILENT_HTTP_CONNECTION_H_	1

#include "connection.h"

namespace silent {


class HttpConnection : public Connection {
public:
	HttpConnection() {}
	~HttpConnection() {}
};
}

#endif

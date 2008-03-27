#ifndef _XF_CONNECTION_H_
#define _XF_CONNECTION_H_	1




namespace xf {


/**
 *	Connection 类
 *
 */
class Connection {
	
private: 
	int fd;				//sock 描述符
	int write_queue;	//数据输出队列..
	
	 
	
	
protected:
	
	Connection() {
	}
	~Connection() {
	}
	
	
template<class,class> friend class Acceptor;	//acceptor 可访问 connection 的所有成员
	
	
};


} // namespace xf;


#endif	// _XF_CONNECTION_H_


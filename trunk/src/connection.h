#ifndef _SILENT_CONNECTION_H_
#define _SILENT_CONNECTION_H_	1


/**
 *	Connection 类所有成员均是 protected 意思是不能直接进行使用的.
 *	包括不能直接创建, 直接删除. 而只能
 *
 */
template<typename UserData>
class Connection {
protected:
	Connection() {
	}
	~Connection() {
	}
	
	int fd;				//sock 描述符
	int write_queue;	//数据输出队列..
	
	

	UserData userdata;

};




#endif


#ifndef _XF_CONNECTION_H_
#define _XF_CONNECTION_H_	1




namespace xf {


/**
 *	Connection ��
 *
 */
class Connection {
	
private: 
	int fd;				//sock ������
	int write_queue;	//�����������..
	
	 
	
	
protected:
	
	Connection() {
	}
	~Connection() {
	}
	
	
template<class,class> friend class Acceptor;	//acceptor �ɷ��� connection �����г�Ա
	
	
};


} // namespace xf;


#endif	// _XF_CONNECTION_H_


#ifndef _SILENT_CONNECTION_H_
#define _SILENT_CONNECTION_H_	1




namespace silent {


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


} // namespace silent;


#endif	// _SILENT_CONNECTION_H_


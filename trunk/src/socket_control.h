#ifndef _SILENT_SOCK_CONTROL_H_
#define _SILENT_SOCK_CONTROL_H_

#ifndef __cplusplus
extern "C" {
#endif

int set_nonblock(int sockfd);
int get_sendbuffer_size(int sockfd);
int get_recvbuffer_size(int sockfd);
int set_sendbuffer_size(int sockfd, int size);
int set_recvbuffer_size(int sockfd, int size);



#ifndef __cplusplus
}
#endif



#endif




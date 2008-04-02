#ifndef _SILENT_SOCK_CONTROL_H_
#define _SILENT_SOCK_CONTROL_H_

#include "os.h"

#ifndef __cplusplus
extern "C" {
#endif

int set_nonblocking(socket_t sockfd);
int get_sendbuffer_size(socket_t sockfd);
int get_recvbuffer_size(socket_t sockfd);
int set_sendbuffer_size(socket_t sockfd, int size);
int set_recvbuffer_size(socket_t sockfd, int size);



#ifndef __cplusplus
}
#endif



#endif




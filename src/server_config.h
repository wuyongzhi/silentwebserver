#ifndef _SILENT_SERVER_CONFIG_H_
#define _SILENT_SERVER_CONFIG_H_


#include "silent_platform.h"


/**		 
 *
 */
typedef struct _server_config {
	uint16_t port;
	uint32_t address;
	int backlog;
}server_config;




#endif


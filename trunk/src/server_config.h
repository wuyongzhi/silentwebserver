#ifndef _SILENT_SERVER_CONFIG_H_
#define _SILENT_SERVER_CONFIG_H_

#include <stdint.h>


/**		 
 *
 */
struct server_config {
	uint16_t port;
	uint32_t address;
	int backlog;
};




#endif


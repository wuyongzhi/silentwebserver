#ifndef _SILENT_CONFIG_H_
#define _SILENT_CONFIG_H_	1


#ifdef USE_NAMESPACE
	#define SILENT_NAMESPACE_BEGIN  silent {
	#define SILENT_NAMESPACE_END    }
	#define USING_SILENT using namespace silent;
#else

	#define SILENT_NAMESPACE_BEGIN  
	#define SILENT_NAMESPACE_END    

#endif


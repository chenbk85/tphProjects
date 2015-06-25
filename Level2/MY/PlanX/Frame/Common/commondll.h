#ifndef COMMON_DLL_H
#define COMMON_DLL_H

#ifndef COMMON_TRANSFER
	#if defined COMMON_EXPORT
        #define COMMON_TRANSFER  __declspec( dllexport ) 
	#elif defined COMMON_IMPORT
        #define COMMON_TRANSFER __declspec( dllimport ) 
	#else
        #define COMMON_TRANSFER
	#endif
#endif

#endif
#ifndef Data_Sybase_INCLUDED
#define Data_Sybase_INCLUDED


#include "Poco/Foundation.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ODBC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ODBC_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(Sybase_EXPORTS)
		#define Sybase_API __declspec(dllexport)
	#else
		#define Sybase_API __declspec(dllimport)
	#endif
#endif


#if !defined(Sybase_API)
	#if !defined(POCO_NO_GCC_API_ATTRIBUTE) && defined (__GNUC__) && (__GNUC__ >= 4)
		#define Sybase_API __attribute__ ((visibility ("default")))
	#else
		#define Sybase_API
	#endif
#endif


//
// Automatically link Data library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Sybase_EXPORTS)
		#pragma comment(lib, "PocoDataSybase" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // Data_Sybase_INCLUDED

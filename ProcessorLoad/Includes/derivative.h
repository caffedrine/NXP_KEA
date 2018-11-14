// Device specific configurations

// This trick with comments really works - remove/add just one '/' at the beginning to swap headers

/*
#define SKEAZ1284
//*/#define SKEAZN642

#ifdef SKEAZ1284
	#include "SKEAZ1284.h"
#elif defined SKEAZN642
	#include "SKEAZN642.h"
#endif


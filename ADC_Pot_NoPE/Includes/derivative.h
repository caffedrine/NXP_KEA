// Device specific configurations

#define SKEAZ1284
//#define SKEAZN642

#ifdef SKEAZ1284
#include "SKEAZ1284.h"
#elif defined SKEAZN642
#include "SKEAZN642.h"
#endif

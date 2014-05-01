#ifndef __UTIL_HEADER__
#define __UTIL_HEADER__

#ifdef DS3_LOG
#define LOG 0==0
#else
#define LOG 0==1
#endif

#define info(...) if(LOG) fprintf(stderr, __VA_ARGS__)

#endif

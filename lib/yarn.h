#ifndef __YARNS_YARN__
#define __YARNS_YARN__

#include "config.h"

typedef unsigned long yarn_t;

yarn_t yarn_current ( void ) __attribute__((pure));
yarn_t yarn_new ( void (*routine)(void*), void* udata, int nice );
void yarn_yield ( yarn_t target );
void yarn_process ( unsigned long otherThreadCount );
#if YARNS_SYNERGY == YARNS_SYNERGY_MARKED
void yarn_mark ();
#else
static inline void yarn_mark () { }
#endif

#endif

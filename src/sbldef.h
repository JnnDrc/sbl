#ifndef _SBLDEF_H
#define _SBLDEF_H

#include <string.h>

#define streq(s1,s2)   !strcmp(s1,s2)
#define memeq(m1,m2,s) !memcmp(m1,m2,s)

#define cast(t,v) (t)(v)

typedef unsigned int uint;

#endif /* _SBLDEF_H */

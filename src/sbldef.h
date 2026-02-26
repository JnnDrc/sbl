#ifndef _SBLDEF_H
#define _SBLDEF_H

#include <string.h>

#define streq(s1,s2)     !strcmp(s1,s2)
#define streqn(s1,s2,l)  !strncmp(s1,s2,l)
#define memeq(m1,m2,s)   !memcmp(m1,m2,s)

#define cast(t,v) (t)(v)

typedef unsigned int uint;

#define abs(x)   ((x) < 0 ? (-x) : (x))
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#define sign(x)  ((x) == 0 ? 0 : (x) < 0 ? -1 : 1)

#define SETF(fs, f) ((fs) |= (f))
#define HASF(fs, f) ((fs) & (f))

#endif /* _SBLDEF_H */

#ifndef _SBLVER_H

// SBL VERSION -----------------------------------------------------------------
#define SBL_MAJOR 0
#define SBL_MINOR 1
// SBL VERSION -----------------------------------------------------------------

#include <stdint.h>

typedef uint16_t sblver_t;

#define SBL_MAKE_VERSION(mj,mn) (sblver_t)((sblver_t)(mj << 8 ) | (mn))
#define SBLVER_MAJOR(v) (uint8_t)((uint16_t)(v) >> 8)
#define SBLVER_MINOR(v) (uint8_t)((uint16_t)(v) & 0xFF)

#endif /* SBLVER_H */

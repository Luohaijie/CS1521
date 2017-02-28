#ifndef _CONFIG_H
#define _CONFIG_H

#include <CS1521.H>

#define TEN_BIT 1
#define LOCK_SELF 0
#define CORRECT_DEBUG 0  //
#define COMS 0x02
#define CSIN 0x03

extern void Port_Init(void);
extern void Time_Init(void);
extern void Iterrup_Init(void);
extern void Power_Init(void);

#endif
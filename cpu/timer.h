#ifndef CPU_TIMER_H_
#define CPU_TIMER_H_

#include <stdint.h>
void init_timer(uint32_t freq);
void sleep(uint32_t time);

#endif /* CPU_TIMER_H_ */

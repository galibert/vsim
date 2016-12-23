#ifndef BLOCKS_H
#define BLOCKS_H

#include "state.h"

void main_timings(cstate &s);
void param_timings(cstate &s);
void phone_input(cstate &s);
void rom_read(cstate &s);
void rom_param_muxer(cstate &s);
void sram_update(cstate &s);
void interpolator(cstate &s);
void right_timings(cstate &s);
void phone_counter(cstate &s);
void phone_counter2(cstate &s);
void clv_detect(cstate &s);
void noise(cstate &s);

#endif

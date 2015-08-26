#ifndef BLOCKS_H
#define BLOCKS_H

#include "state.h"

void main_timings(cstate &s);
void param_timings(cstate &s);
void phone_input(cstate &s);
void rom_read(cstate &s);
void rom_param_muxer(cstate &s);

#endif

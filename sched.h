#ifndef SCHED_H
#define SCHED_H

#include "state.h"
#include <list>

void compute_sched(const cstate &r, const cstate &s, std::list<void (*)(cstate &)> &f, bool first);

#endif


#ifndef SCHED_H
#define SCHED_H

#include "state.h"
#include <set>

void compute_sched(const cstate &r, const cstate &s, std::set<void (*)(cstate &)> &f);

#endif


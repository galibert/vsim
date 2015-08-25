#include "sched.h"
#include "blocks.h"

#define T(c) r.c != s.c

void compute_sched(const cstate &r, const cstate &s, std::set<void (*)(cstate &)> &f)
{
  if(T(clk_0) || T(clk_1))
    f.insert(main_timings);
  if(T(clk_0) || T(clk_1) || T(param_timing_sr_enable))
    f.insert(param_timings);
}

#include "sched.h"
#include "blocks.h"

#define T(c) r.c != s.c

void compute_sched(const cstate &r, const cstate &s, std::set<void (*)(cstate &)> &f)
{
  if(T(clk_0) || T(clk_1))
    f.insert(main_timings);
  if(T(clk_0) || T(clk_1) || T(param_timing_sr_enable))
    f.insert(param_timings);
  if(T(pad_stb) || T(phi1) || T(p_input) || T(rom_extra))
    f.insert(phone_input);
  if(T(p_rom) || T(rom_hsel_f1) || T(rom_hsel_va) || T(rom_hsel_f2) || T(rom_hsel_fc) || T(rom_hsel_f2q) || T(rom_hsel_f3) || T(rom_hsel_fa))
    f.insert(rom_read);
  if(T(gtsr[1]) || T(gtsr[3]) || T(gtsr[5]) || T(rom_param))
    f.insert(rom_param_muxer);
}

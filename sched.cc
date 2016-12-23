#include "sched.h"
#include "blocks.h"

#define T(c) r.c != s.c

void compute_sched(const cstate &r, const cstate &s, std::list<void (*)(cstate &)> &f, bool first)
{
  if(first || T(clk_0) || T(clk_1))
    f.push_back(main_timings);
  if(first || T(clk_0) || T(clk_1) || T(param_timing_sr_enable))
    f.push_back(param_timings);
  if(first || T(pad_stb) || T(phi1) || T(p_input) || T(rom_extra) || T(input_phone_latch_stb))
    f.push_back(phone_input);
  if(first || T(p_rom) || T(rom_hsel_f1) || T(rom_hsel_va) || T(rom_hsel_f2) || T(rom_hsel_fc) || T(rom_hsel_f2q) || T(rom_hsel_f3) || T(rom_hsel_fa))
    f.push_back(rom_read);
  if(first || T(gtsr[1]) || T(gtsr[3]) || T(gtsr[5]) || T(rom_param))
    f.push_back(rom_param_muxer);
  if(first || T(sram_w) || T(sram_enable_w) ||T(rom_hsel_f1) || T(rom_hsel_va) || T(rom_hsel_f2) || T(rom_hsel_fc) || T(rom_hsel_f2q) || T(rom_hsel_f3) || T(rom_hsel_fa) || T(gtsr[1]) || T(gtsr[3]) || T(gtsr[5]) || T(param_timing_sr_enable) || T(clk_1) || T(pulse_80Hz))
    f.push_back(sram_update);
  if(first || T(sram_r) || T(sram_alt_r) || T(clk_0) || T(clk_1))
    f.push_back(interpolator);
  if(first || T(rom_hsel_f1) || T(rom_hsel_va))
    f.push_back(right_timings);
  if(first || T(phi1) || T(phi2) || T(phc2[0]) || T(input_phone_latch_rom) || T(phone_end))
    f.push_back(phone_counter);     
  if(first || T(phi1) || T(phi2) || T(tick_ph) || T(input_phone_latch_rom) || T(rom_clvd))
    f.push_back(phone_counter2);
  if(first || T(input_phone_latch_rom) || T(clvd_detect) || T(pulse_80Hz) || T(rom_hsel_f1) || T(rom_hsel_fa) || T(rom_hsel_fc) || T(pause) || T(silent) || T(tick_625hz) || T(tick_208hz))
    f.push_back(clv_detect);
  if(first || T(phi1) || T(phi2))
    f.push_back(noise);
}

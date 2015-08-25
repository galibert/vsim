#include "blocks.h"

void main_timings(cstate &s)
{
  s.gtsr_enable_input_1 = !(s.gtsr[1] || s.gtsr[3] || !s.gtsr[5]);
  s.gtsr_input = (s.gtsr[3]^s.gtsr[5]) || (s.gtsr[1] && s.gtsr[3]);
    
  if(!s.clk_1) {
    s.gtsr[1] = !s.gtsr[0];
    s.gtsr[3] = !s.gtsr[2];
    s.gtsr[5] = !s.gtsr[4];
    s.gtsr_enable_input_2 = !s.param_timing_sr_enable;
    s.final_filter_clock = !s.gtsr_enable_input_2;
  }
  if(!s.clk_0) {
    if(!s.gtsr_enable_input_1 && !s.gtsr_enable_input_2 && !s.clk_0)
      s.gtsr[0] = s.gtsr_input;
    s.gtsr[2] = !s.gtsr[1];
    s.gtsr[4] = !s.gtsr[3];
    s.param_timing_sr_enable = !s.gtsr_enable_input_1;
  }
}

void param_timings(cstate &s)
{
  if(!s.clk_1 && !s.param_timing_sr_enable)
    for(int i=1; i<17; i+=2)
      s.paramsr[i] = !s.paramsr[i-1];

  if(!s.clk_0)
    for(int i=2; i<17; i+=2)
      s.paramsr[i] = !s.paramsr[i-1];

  s.rom_hsel_f2  = !s.paramsr[1]  || s.paramsr[2];
  s.rom_hsel_f2q = !s.paramsr[3]  || s.paramsr[4];
  s.rom_hsel_f3  = !s.paramsr[5]  || s.paramsr[6];
  s.rom_extra    = !s.paramsr[7]  || s.paramsr[8];
  s.rom_hsel_fc  = !s.paramsr[9]  || s.paramsr[10];
  s.rom_hsel_fa  = !s.paramsr[11] || s.paramsr[12];
  s.rom_hsel_va  = !s.paramsr[13] || s.paramsr[14];
  s.rom_hsel_f1  = !s.paramsr[15] || s.paramsr[16];

  if(!s.clk_0)
    s.paramsr[0] = !(s.rom_hsel_f2 && s.rom_hsel_f2q && s.rom_hsel_f3 && s.rom_extra && s.rom_hsel_fc && s.rom_hsel_fa && s.rom_hsel_va);

  bool t = !(!s.phi1 && !s.paramsr[5] && !s.paramsr[13]);
  s.phi1 = (t && s.rom_hsel_f2 && s.rom_hsel_fc);

  t = !(!s.phi2 && !s.paramsr[1] && !s.paramsr[9]);
  s.phi2 = (t && s.rom_hsel_f3 && s.rom_hsel_va);
}

#include "blocks.h"
#include <stdio.h>

void sram_update(cstate &s)
{
  bool driver_latch = !(s.gtsr[3] && s.gtsr[5] && s.clk_1);

  bool sram_io_clear = !(s.param_timing_sr_enable || s.clk_1);
  int slot = -1, alt_slot = -1;
  switch(s.gtsr[1]*4 + s.gtsr[3]*2 + s.gtsr[5]) {
  case 6: slot = 0; alt_slot = 5; break;
  case 4: slot = 1; alt_slot = 6; break;
  case 0: slot = 2; alt_slot = 7; break;
  case 1: slot = 3; alt_slot = 0; break;
  case 2: slot = 4; alt_slot = 1; break;
  case 5: slot = 5; alt_slot = 2; break;
  case 3: slot = 6; alt_slot = 3; break;
  case 7: slot = 7; alt_slot = 4; break;
  }
  slot = (slot + 5) & 7;
  alt_slot = (alt_slot + 5) & 7;

  int line = -1;
  if(!s.rom_hsel_f1 ) line = 0;
  if(!s.rom_hsel_va ) line = 1;
  if(!s.rom_hsel_f2 ) line = 2;
  if(!s.rom_hsel_fc ) line = 3;
  if(!s.rom_hsel_f2q) line = 4;
  if(!s.rom_hsel_f3 ) line = 5;
  if(!s.rom_hsel_fa ) line = 6;

  if(sram_io_clear && line != -1)
    s.sram[line] = 0x00;

  if(!s.pulse_80Hz)
    s.sram_enable_hold = !s.sram_enable_w;

  if(line != -1 && s.clk_1 && s.pulse_80Hz && s.sram_enable_hold) {
    if(s.sram_w)
      s.sram[line] = s.sram[line] | (1 << slot);
    else
      s.sram[line] = s.sram[line] & ~(1 << slot);
  }

  s.sram_r = !(s.sram[line] & (1 << slot));
  s.sram_alt_r = alt_slot < 5 ? !(s.sram[line] & (1 << alt_slot)) : true;

  if(!driver_latch)
    s.driver_in = line == -1 ? 0x1f : (~s.sram[line]) & 0x1f;
}

void interpolator(cstate &s)
{
  if(!s.clk_1) {
    s.latch_sram_r = s.sram_r;
    s.latch_sram_alt_r = s.sram_alt_r;
    s.carry1_out = s.carry1_in;
    s.carry2_out = s.carry2_in;
  }

  bool p263 = !s.latch_sram_r^s.latch_sram_alt_r;
  bool p217 = !p263^s.carry1_out;
  bool p182 = !p217^s.rom_muxed_fx_out;
  s.sram_w = p182^s.carry2_out;

  if(!s.clk_0) {
    s.carry1_in =
      s.pulse_80Hz && ((p263 || s.carry1_out) && (s.latch_sram_r || s.latch_sram_alt_r));
    s.carry2_in =
      (!s.pulse_80Hz) || ((p182 || s.carry2_out) && (p217 || s.rom_muxed_fx_out));
  }
}

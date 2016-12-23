#include "blocks.h"

void main_timings(cstate &s)
{
  s.gtsr_enable_input = !(s.gtsr[1] || s.gtsr[3] || !s.gtsr[5]);
  s.gtsr_input = (s.gtsr[3]^s.gtsr[5]) || (s.gtsr[1] && s.gtsr[3]);

  // Hack to get the full cycle and not miss '111'
  if(s.gtsr[1] && s.gtsr[3] && !s.gtsr[5])
    s.gtsr_input = false;

  if(!s.clk_1) {
    s.gtsr[1] = !s.gtsr[0];
    s.gtsr[3] = !s.gtsr[2];
    s.gtsr[5] = !s.gtsr[4];
    s.pulse_80Hz = s.param_timing_sr_enable;
  }
  if(!s.clk_0) {
    if(!s.gtsr_enable_input && s.pulse_80Hz && !s.clk_0)
      s.gtsr[0] = s.gtsr_input;
    s.gtsr[2] = !s.gtsr[1];
    s.gtsr[4] = !s.gtsr[3];
    s.param_timing_sr_enable = !s.gtsr_enable_input;
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

  bool t = s.phi1 || s.paramsr[5] || s.paramsr[13];
  s.phi1 = t && s.rom_hsel_f2 && s.rom_hsel_fc;

  t = s.phi2 || s.paramsr[1] || s.paramsr[9];
  s.phi2 = (t && s.rom_hsel_f3 && s.rom_hsel_va);
}

void phone_input(cstate &s)
{
  if(!s.rom_extra)
    s.stbsr[1] = !s.stbsr[0];

  if(!s.phi1)
    s.stbsr[2] = !s.stbsr[1];

  bool t = !s.input_phone_latch_stb || !s.stbsr[2];
  s.input_phone_latch_stb = !t || !s.pad_stb;
  s.input_phone_latch_rom = !(s.stbsr[1] && s.stbsr[2] && !s.rom_extra);

  if(!s.phi1)
    s.stbsr[0] = !s.input_phone_latch_stb;

  if(!s.input_phone_latch_stb)
    s.p_stb = s.p_input;
  if(!s.input_phone_latch_rom)
    s.p_rom = s.p_stb;
}

void right_timings(cstate &s)
{
  // 0..11
  if(!s.rom_hsel_f1)
    s.right1[1] = s.right1[0];
  if(!s.rom_hsel_va)
    s.right1[0] = !s.right1[1];

  bool changed2;
  do {
    changed2 = false;
    for(int i=2; i<12; i+=2) {
      bool changed;
      do {
	changed = false;
	if((i == 8 || i == 10) && !s.right2[0]) {
	  if(!s.right1[9] || !s.right1[10]) {
	    changed2 = changed = true;
	    s.right1[9] = s.right1[10] = true;
	  }
	}
	if(!s.right1[i-1]) {
	  if(s.right1[i] != !s.right1[i+1]) {
	    changed2 = changed = true;
	    s.right1[i] = !s.right1[i+1];
	  }
	} else {
	  if(s.right1[i+1] != s.right1[i]) {
	    changed2 = changed = true;
	    s.right1[i+1] = s.right1[i];
	  }
	}
      } while(changed);

      if(!s.rom_hsel_va) {
	bool nv = !s.right1[8] || s.right1[11] || s.right1[7];
	if(s.right2[1] != !nv) {
	  changed2 = true;
	  s.right2[1] = !nv;
	}
      }
      if(!s.rom_hsel_f1) {
	if(s.right2[0] != !s.right2[1]) {
	  changed2 = true;
	  s.right2[0] = !s.right2[1];
	}
      }
    }
  } while(changed2);

  if(!s.rom_hsel_va)
    s.right3[3] = !s.right1[7];
  if(!s.rom_hsel_f1)
    s.right3[2] = !s.right3[3];
  if(!s.rom_hsel_va)
    s.right3[1] = !s.right3[2];
  if(!s.rom_hsel_f1)
    s.right3[0] = !s.right3[1];

  s.tick_625hz = s.right3[0] || !s.right3[2];
  s.tick_208hz = s.right2[0];
}

void phone_counter(cstate &s)
{
  bool phonetime_counter_reset = s.input_phone_latch_rom && (s.phi2 || s.phc2[0]);
  if(!phonetime_counter_reset)
    for(int i=0; i<18; i+=2)
      s.phc[i] = true;
  if(!s.phi1)
    s.phc[0] = s.phc[1];
  if(!s.phi2)
    s.phc[1] = !s.phc[0];

  for(int i=2; i<18; i+=2) {
    bool changed;
    do {
      changed = false;
      if(s.phc[i-1]) {
	if(phonetime_counter_reset && s.phc[i] != s.phc[i+1]) {
	  changed = true;
	  s.phc[i] = s.phc[i+1];
	}
      } else {
	if(s.phc[i+1] != !s.phc[i]) {
	  changed = true;
	  s.phc[i+1] = !s.phc[i];
	}
      }
    } while(changed);
  }

  s.tick_ph = !s.phone_end;
  for(int i=0; i<7; i++)
    if(s.phc[5+2*i] != ((s.rom_duration >> i) & 1))
      s.tick_ph = true;
}

void phone_counter2(cstate &s)
{
  if(!s.input_phone_latch_rom)
    for(int i=0; i<12; i+=2)
      s.phc2[i] = true;
  if(!s.phi1)
    s.phc2[0] = s.tick_ph;
  if(!s.phi2)
    s.phc2[1] = !s.phc2[0];

  for(int i=2; i<12; i+=2) {
    bool changed;
    do {
      changed = false;
      if(s.phc2[i-1]) {
	if(s.input_phone_latch_rom && s.phc2[i] != s.phc2[i+1]) {
	  changed = true;
	  s.phc2[i] = s.phc2[i+1];
	}
      } else {
	if(s.phc2[i+1] != !s.phc2[i]) {
	  changed = true;
	  s.phc2[i+1] = !s.phc2[i];
	}
      }
    } while(changed);
  }
  s.phone_end = !s.phc2[11];

  s.clvd_detect = false;
  for(int i=0; i<4; i++)
    if(s.phc2[3+2*i] != ((s.rom_clvd >> i) & 1))
      s.clvd_detect = true;
}

void clv_detect(cstate &s)
{
  if(!s.input_phone_latch_rom)
    s.cv_reached = s.cl_reached = true;
  else if(!s.clvd_detect && !s.pulse_80Hz) {
    if(!s.rom_hsel_f1)
      s.cv_reached = false;
    if(!s.rom_hsel_fa)
      s.cl_reached = false;
  }
  if(!s.cl_reached)
    s.cl_value = s.rom_cl;

  // 1.2, 1.5
  s.clc_reset = s.rom_extra || !(s.cl_value || s.silent);

  // 2.1, 2.2, 2.3, 2.4, 1.1, 2.5, 1.4, 1.3
  s.sram_enable_w =
    (((s.cl_reached || s.rom_hsel_fc) && (s.cv_reached || s.rom_hsel_fa)) || s.tick_625hz) && 
    (!s.rom_hsel_fc || !s.rom_hsel_fa || !(s.pause || s.silent) || s.tick_208hz);
}

void noise(cstate &s)
{
  static int zz = 0;
  zz++;
  bool prev_i = false, prev_o = false, first = true;
  for(;;) {
    s.noise_out = !s.noise[29]^s.noise[27];
    bool line = false;
    for(int i=1; i<30; i+=2)
      if(s.noise[i])
	line = true;
    bool inp = (zz < 100) || !(line && s.noise_out);
    if(!first && prev_i == inp && prev_o == s.noise_out)
      return;
    prev_i = inp;
    prev_o = s.noise_out;
    first = false;

    if(!s.phi2) {
      s.noise[0] = !inp;
      for(int i=2; i<30; i+=2)
	s.noise[i] = !s.noise[i-1];
    }
    if(!s.phi1) {
      for(int i=1; i<30; i+=2)
	s.noise[i] = !s.noise[i-1];
    }
  }
}

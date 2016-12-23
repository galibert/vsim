#ifndef STATE_H
#define STATE_H

struct cstate {
  bool first;

  unsigned long long ctime;
  bool clk_0, clk_1;
  bool gtsr[6];
  bool gtsr_input;
  bool gtsr_enable_input;
  bool pulse_80Hz;
  bool param_timing_sr_enable;
  bool final_filter_clock;
  bool paramsr[17];
  bool rom_extra, rom_hsel_f1, rom_hsel_f2, rom_hsel_f2q, rom_hsel_f3, rom_hsel_fa, rom_hsel_fc, rom_hsel_va;
  bool phi1, phi2;
  bool stbsr[3];
  bool input_phone_latch_stb, input_phone_latch_rom, pad_stb;
  unsigned char p_input, p_stb, p_rom;
  unsigned char rom_param, rom_clvd, rom_duration;
  bool rom_cl;
  bool rom_muxed_fx_out;
  bool sram_w, sram_enable_w, sram_enable_hold, sram_r, sram_alt_r;
  bool latch_sram_r, latch_sram_alt_r, carry1_in, carry1_out, carry2_in, carry2_out;
  unsigned char sram[7];
  bool driver_in;
  bool right1[12];
  bool right2[2];
  bool right3[4];
  bool tick_625hz, tick_208hz, tick_ph;
  bool phone_end, clvd_detect;
  bool phc[18], phc2[12];
  bool cv_reached, cl_reached, cl_value;
  bool pause, silent, clc_reset;
  bool noise[30];
  bool noise_out;
};

#endif

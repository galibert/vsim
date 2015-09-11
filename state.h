#ifndef STATE_H
#define STATE_H

struct cstate {
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
  unsigned char sram[7];
  bool driver_in;
};

#endif

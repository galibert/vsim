#ifndef STATE_H
#define STATE_H

struct cstate {
  unsigned long long ctime;
  bool clk_0, clk_1;
  bool gtsr[6];
  bool gtsr_input;
  bool gtsr_enable_input_1;
  bool gtsr_enable_input_2;
  bool param_timing_sr_enable;
  bool final_filter_clock;
  bool paramsr[17];
  bool rom_extra, rom_hsel_f1, rom_hsel_f2, rom_hsel_f2q, rom_hsel_f3, rom_hsel_fa, rom_hsel_fc, rom_hsel_va;
  bool phi1, phi2;
};

#endif
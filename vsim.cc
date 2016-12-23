#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <list>

#include "blocks.h"
#include "sched.h"

cstate ref;

void init(cstate &s)
{
  memset(&s, 0, sizeof(s));
  ref = s;
  s.first = true;
  s.silent = false;
}

void update(cstate &s)
{
  for(;;) {
    std::list<void (*)(cstate &)> f;
    compute_sched(ref, s, f, s.first);
    s.first = false;
    ref = s;
    if(f.empty())
      break;
    for(std::list<void (*)(cstate &)>::const_iterator i = f.begin(); i != f.end(); i++)
      (*i)(s);
  }
}

static unsigned char swp(unsigned char v)
{
  v = ((v >> 4) & 0x0f) | ((v << 4) & 0xf0);
  v = ((v >> 2) & 0x33) | ((v << 2) & 0xcc);
  v = ((v >> 1) & 0x55) | ((v << 1) & 0xaa);
  return v;
}

void show_state(const cstate &s)
{
  static bool xphi1 = true, xphi2=true;
  if(true || xphi1 != s.phi1 || xphi2 != s.phi2) {
    xphi1 = s.phi1;
    xphi2 = s.phi2;
    printf("%02d.%06d.%d: gtsr=%d%d%d(%d) p80=%d",
	   int(s.ctime/720000/2), int((s.ctime/2) % 720000), int(s.ctime & 1),
	   s.gtsr[1], s.gtsr[3], s.gtsr[5], !s.gtsr_input,
	   s.pulse_80Hz);
    printf(" rom=");
    bool f=true;
    if(!s.rom_hsel_f2) { if(!f) printf(","); printf("f2 "); f=false; }
    if(!s.rom_hsel_f2q){ if(!f) printf(","); printf("f2q"); f=false; }
    if(!s.rom_hsel_f3) { if(!f) printf(","); printf("f3 "); f=false; }
    if(!s.rom_extra)   { if(!f) printf(","); printf("xt "); f=false; }
    if(!s.rom_hsel_fc) { if(!f) printf(","); printf("fc "); f=false; }
    if(!s.rom_hsel_fa) { if(!f) printf(","); printf("fa "); f=false; }
    if(!s.rom_hsel_va) { if(!f) printf(","); printf("va "); f=false; }
    if(!s.rom_hsel_f1) { if(!f) printf(","); printf("f1 "); f=false; }
    if(f)
      printf("   ");

    if(0)
      printf(" phi1=%d phi2=%d stbsr=%d%d%d pad_stb=%d ipl_s=%d ipl_r=%d p_stb=%02x p_rom=%02x rp=%x rclvd=%x rd=%02x rcl=%d rmux=%d",
	     s.phi1, s.phi2,
	     s.stbsr[0], !s.stbsr[1], s.stbsr[2],
	     s.pad_stb,
	     s.input_phone_latch_stb, s.input_phone_latch_rom,
	     s.p_stb, s.p_rom,
	     s.rom_param, s.rom_clvd, s.rom_duration, s.rom_cl,
	     s.rom_muxed_fx_out
	     );

    if(0)
      printf(" sram=%02x.%02x.%02x.%02x.%02x.%02x.%02x di=%02x srew=%d clcr=%d cvr=%d clr=%d clv=%d",
	     swp(s.sram[0]), swp(s.sram[1]), swp(s.sram[2]), swp(s.sram[3]), swp(s.sram[4]), swp(s.sram[5]), swp(s.sram[6]),
	     s.driver_in,
	     s.sram_enable_w, s.clc_reset, s.cv_reached, s.cl_reached, s.cl_value);
    if(0)
      printf(" 625Hz=%d 208hz=%d",
	     s.tick_625hz, s.tick_208hz);
    if(0)
      printf(" phc=%02x.%d pht=%d phc2=%d.%x.%d clvd_d=%d",
	   (s.phc[17] ? 0x40 : 0) |
	   (s.phc[15] ? 0x20 : 0) |
	   (s.phc[13] ? 0x10 : 0) |
	   (s.phc[11] ? 0x08 : 0) |
	   (s.phc[ 9] ? 0x04 : 0) |
	   (s.phc[ 7] ? 0x02 : 0) |
	   (s.phc[ 5] ? 0x01 : 0),
	   (s.phc[ 3] ? 0x02 : 0) |
	   (s.phc[ 1] ? 0x01 : 0),
	   s.tick_ph,
	   s.phc2[11],
	   (s.phc2[9] ? 0x8 : 0) |
	   (s.phc2[7] ? 0x4 : 0) |
	   (s.phc2[5] ? 0x2 : 0) |
	   (s.phc2[3] ? 0x1 : 0),
	   s.phc2[1],
	   s.clvd_detect);

    if(0)
      printf(" add=%d%d%d c1=%d%d c2=%d%d w=%d",
	     !s.latch_sram_r, s.latch_sram_alt_r, !s.rom_muxed_fx_out, !s.carry1_in, !s.carry1_out, !s.carry2_in, !s.carry2_out, s.sram_w);

    if(1) {
      printf(" noise=");
      for(int i=0; i<30; i+=2)
	printf("%d", s.noise[i]);
      printf(" ");
      for(int i=1; i<30; i+=2)
	printf("%d", s.noise[i]);
      printf(" no=%d", s.noise_out);
    }
    printf("\n");
  }
}

void step(cstate &s, bool verbose)
{
  s.clk_0 = false;
  update(s);
  if(verbose)
    show_state(s);
  s.clk_0 = true;
  update(s);
  s.ctime++;
  s.clk_1 = false;
  update(s);
  if(false && verbose)
    show_state(s);
  s.clk_1 = true;
  update(s);
  s.ctime++;
}

int main()
{
  cstate s;
  init(s);

  s.pad_stb = false;
  s.p_stb = s.p_rom = s.p_input = 0x3f;
  s.clk_0 = s.clk_1 = true;
  for(int i=0; i<1000; i++)
    step(s, true);

  s.p_input = 0x20;
  s.pad_stb = true;
  for(int i=0; i<72; i++)
    step(s, true);
  s.pad_stb = false;
  for(int i=0; i<43; i++)
    step(s, true);
  for(int i=0; i<500000; i++)
    step(s, true);

  return 0;
}
